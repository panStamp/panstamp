#!/usr/bin/env python2
# encoding: utf-8
# vim: ts=4 noexpandtab

import urwid
import urwid.raw_display
import sys

import re, sys, random
from sorteddict import SortedDict

import modules
import field_encodings

DATA = SortedDict()
WIDMAP = {}

# GENERAL CONFIG ############################################################

DATA["TEXT_GENERAL"] = {
	"name": "General options",
	"type": "info",
}

DATA["CONFIG_DEBUG"] = {
	"name": "Build debug code",
	"default": False,
	"help": "Sets CFLAGS and LDFLAGS for debugging",
}

DATA["CONFIG_FIXEDPOINT_MATH"] = {
	"name": "Fixedpoint Math (EXPERIMENTAL)",
	"default": False,
	"help": "Tries to use fix point aritmetric. If no module is using it, it reduces the code size dramaticly.",
}

DATA["USE_LCD_CHARGE_PUMP"] = {
	"name": "Use LCD Charge Pump (6 bytes)",
	"default": False,
	"help": "Use the internal charge pump to make the display contrast contstant through the whole battery lifetime. As a downside this increases currency and reduces battery lifetime.",
}

DATA["USE_WATCHDOG"] = {
	"name": "Use Watchdog (20 bytes)",
	"default": True,
	"help": "Protects the clock against deadlocks by rebooting it.",
}

# RTC DRIVER #################################################################

DATA["TEXT_RTC"] = {
	"name": "RTC driver",
	"type": "info",
}

DATA["CONFIG_RTC_IRQ"] = {
	"name": "Enable RTC interrupts",
	"default": True,
	"help": "Enables interrupts on the Real Time Clock",
}

DATA["CONFIG_RTC_DST"] = {
	"name": "DST",
	"default": True,
	'depends': [ 'CONFIG_RTC_IRQ' ],
	"help": "Automatically adjusts real-time clock for daylight savings time",
}

DATA["CONFIG_RTC_DST_ZONE"] = {
	"name": "DST zone for automatic adjustment",
	"type": "text",
	"default": 1,
	'depends': [ 'CONFIG_RTC_DST' ],
	"help": "DST Zone: 1=DST_US, 2=DST_MEX, 3=DST_BRZ, 4=DST_EU, 5=DST_AUS, 6=DST_NZ"
}

# TIMER0 DRIVER ##############################################################

DATA["TEXT_TIMER"] = {
	"name": "Timer0 driver",
	"type": "info",
}

DATA["CONFIG_TIMER_4S_IRQ"] = {
	"name": "Enable 0.244Hz timer interrupts",
	"default": True,
	"help": "Enables 0.244Hz interrupts on the hardware timer",
}

DATA["CONFIG_TIMER_20HZ_IRQ"] = {
	"name": "Enable 20Hz timer interrupts",
	"default": True,
	"help": "Enables 20Hz interrupts on the hardware timer",
}

# PORTS DRIVER ###############################################################

DATA["TEXT_PORTS"] = {
	"name": "Ports driver",
	"type": "info",
}

DATA["CONFIG_BUTTONS_LONG_PRESS_TIME"] = {
	"name": "Button long press time",
	"type": "text",
	"default": "6",
	"ifndef": True,
	"help": "Long button press time (in multiples of 1/20 second)",
}

DATA["CONFIG_BUTTONS_SHORT_PRESS_TIME"] = {
	"name": "Button short press time",
	"type": "text",
	"default": "1",
	"ifndef": True,
	"help": "Short button press time (in multiples of 1/20 second)",
}

# BATTERY DRIVER #############################################################

DATA["TEXT_BATTERY"] = {
	"name": "Battery driver",
	"type": "info",
}

DATA["CONFIG_BATTERY_MONITOR"] = {
	"name": "Background Battery Monitor",
	"default": False,
	'depends': [ 'CONFIG_RTC_IRQ' ],
	"help": "Monitors the battery voltage every minute and displays a warning on low battery. Also used by the battery to auto-refresh display, if enabled.",
}

DATA["CONFIG_BATTERY_DISABLE_FILTER"] = {
	"name": "Disables battery filter [FOR TESTING]",
	"default": True,
	"help": "Reports the straight voltage value from measurement instead of the 'smoothed' one.",
}

# TEMPERATURE SENSOR DRIVER ##################################################

DATA["TEXT_TEMPERATURE"] = {
	"name": "Temperature sensor driver",
	"type": "info"
}

DATA["CONFIG_TEMPERATURE_OFFSET"] = {
	"name": "Compensation offset",
    "type": "text",
	"default": "-260",
	"ifndef": True,
	"help": "Compensate the sensor raw output value with this offset",
}

DATA["CONFIG_TEMPERATURE_METRIC"] = {
	"name": "Show in degrees C",
	"default": True,
	"help": "Show in degrees C if enabled, F otherwise.",
}

# AUTOMATICALLY GENERATED MODULE LIST ########################################

DATA["TEXT_MODULES"] = {
	"name": "Modules",
	"type": "info"
}

for key,field in modules.read_config():
	DATA[key] = field


HEADER = """
#ifndef _CONFIG_H_
#define _CONFIG_H_

"""

FOOTER = """
#endif // _CONFIG_H_
"""

class HelpListWalker(urwid.SimpleListWalker):
	def __init__(self, app, *args, **kwargs):
		self.app = app
		super(HelpListWalker, self).__init__(*args, **kwargs)

	def set_focus(self, focus):
		if not hasattr(self[focus], "_widget"):
			self.app.help_widget.set_text("")
		else:
			wid = self[focus]._widget
			if "help" in wid._datafield:
				self.app.help_widget.set_text(wid._datafield["help"])
		return super(HelpListWalker, self).set_focus(focus)

def widget_changed_callback(wid, state):
	global WIDMAP, DATA

	cfgname = wid._datakey

	deps = []
	for key,field in DATA.iteritems():
		if not 'depends' in field:
			continue

		if cfgname in field['depends']:
			deps.append(key)

	changeddeps = []
	for depkey in deps:
		depwid = WIDMAP[depkey]
		if isinstance(depwid, urwid.Padding):
			depwid = depwid.original_widget

		psel = depwid.original_widget.selectable()
		if state and wid.selectable():
			depwid.set_attr_map({None: 'opt'})
			depwid.original_widget._selectable = True
		else:
			depwid.set_attr_map({None: 'optd'})
			depwid.original_widget._selectable = False
			changeddeps.append( depwid.original_widget )

		if psel != depwid.original_widget.selectable():
			changeddeps.append( depwid.original_widget )

	# recursively disable widgets depending on the ones we just disabled
	for depwid in changeddeps:
		if isinstance(depwid, urwid.Edit):
			val = depwid.get_edit_text()
		elif isinstance(depwid, urwid.CheckBox):
			val = depwid.get_state()
		else:
			continue
		widget_changed_callback(depwid, val)

class CheckBoxWidget(urwid.CheckBox):
	def __init__(self, *args, **kwargs):
		self._selectable = True
		urwid.CheckBox.__init__(self, *args, **kwargs)

	def selectable(self):
		return self._selectable

class EditWidget(urwid.Edit):
	def __init__(self, *args, **kwargs):
		self._selectable = True
		urwid.Edit.__init__(self, *args, **kwargs)

	def selectable(self):
		return self._selectable

class OpenChronosApp(object):
	def main(self):
		global DATA

		text_header = (u"OpenChronos config  \u2503  "
					   u"UP / DOWN / PAGE UP / PAGE DOWN scroll.  F8 aborts.")

		self.list_content = list_content = []
		for key,field in DATA.iteritems():
			# generate gui forms depending on type
			self.generate_widget(key,field)

		# rescan widgets and disable/enable them based on their dependencies
		for key,wid in WIDMAP.iteritems():
			if not hasattr(wid, '_widget'):
				continue
			wid = wid._widget
			if isinstance(wid, urwid.Edit):
				val = wid.get_edit_text()
			elif isinstance(wid, urwid.CheckBox):
				val = wid.get_state()
			else:
				continue
			widget_changed_callback(wid, val)

		def ok_pressed(*args, **kwargs):
			raise urwid.ExitMainLoop()

		def abort_pressed(*args, **kwargs):
			sys.exit(0)

		list_content.append(urwid.Divider(div_char=u"\u2550", top=1, bottom=1))
		list_content.append(
		urwid.Padding(urwid.GridFlow(
			[urwid.AttrWrap(urwid.Button("Save", ok_pressed), 'opt','optsel'),
			 urwid.AttrWrap(urwid.Button("Abort", abort_pressed), 'opt','optsel')],
			15, 4, 4, 'center'),
			('fixed left',4), ('fixed right',3)))


		header = urwid.AttrWrap(urwid.Text(text_header), 'header')
		walker = HelpListWalker(self, list_content)
		listbox = urwid.ListBox(walker)
		self.help_widget = urwid.Text("")
		footer = urwid.AttrWrap(self.help_widget, 'footer')
		frame = urwid.Frame(urwid.AttrWrap(listbox, 'body'), header=header, footer=footer)

		screen = urwid.raw_display.Screen()
		palette = [
			('header',	'white',	'dark red', 	'bold'),
			('info',	'black',	'dark cyan'),
			('body',	'black',	'dark gray'),
			('optsel',	'white',	'dark blue'),
			('opt',		'black',	'light gray'),
			('optd',	'black',	'dark gray'),
			]


		def unhandled(key):
			if key == 'f8':
				#raise urwid.ExitMainLoop()
				sys.exit(0)

		urwid.MainLoop(frame, palette, screen,
			unhandled_input=unhandled).run()

	def generate_widget(self, key, field):
		global WIDMAP

		if field.get("type", "bool") == "bool":
			wid = CheckBoxWidget(field["name"], field["value"])
			urwid.connect_signal(wid, "change", widget_changed_callback)
			wid._datakey = key
			wid._datafield = field
			f = urwid.AttrWrap(wid, 'opt','optsel')
			if field.has_key('ischild') and field['ischild']:
				f = urwid.Padding(f, width=77, left=3)
			f._widget = wid
			WIDMAP[key] = f
			self.list_content.append(f)

		elif field["type"] == "text":
			wid = EditWidget("%s: "%field["name"], str(field["value"]))
			urwid.connect_signal(wid, "change", widget_changed_callback)
			wid._datakey = key
			wid._datafield = field
			f = urwid.AttrWrap(wid, 'opt', 'optsel')
			if field.has_key('ischild') and field['ischild']:
				f = urwid.Padding(f, width=77, left=3)
			f._widget = wid
			WIDMAP[key] = f
			self.list_content.append(f)

		elif field["type"] == "info":
			wid = urwid.Text(field["name"])
			wid._datakey = key
			wid._datafield = field
			f = urwid.AttrWrap(wid, 'info', 'info')
			f._widget = wid
			WIDMAP[key] = f
			self.list_content.append(f)

	def get_config(self):
		return DATA

	def save_config(self):
		global WIDMAP, DATA

		for key,field in WIDMAP.iteritems():
			if not hasattr(field, '_widget'):
				continue

			wid = field._widget

			if isinstance(wid, urwid.Edit):
				DATA[key]["value"] = wid.get_edit_text()
			elif isinstance(wid, urwid.CheckBox):
				DATA[key]["value"] = wid.get_state()

		fp = open("config.h", "w")
		fp.write("// !!!! DO NOT EDIT !!!, use: make config\n")
		fp.write(HEADER)
		for key,dat in DATA.iteritems():
			if not "value" in dat:
				continue
			if "type" in dat and dat["type"] == "info":
				continue
			if "encoding" in dat:
				fun = getattr(field_encodings, dat['encoding'])
				dat["value"] = fun(dat["value"], True)
			if DATA[key].get("ifndef", False):
				fp.write("#ifndef %s\n" %key)
			if isinstance(dat["value"], bool):
				if dat["value"]:
					fp.write("#define %s\n" %key)
				else:
					fp.write("// %s is not set\n" %key)
			else:
				fp.write("#define %s %s\n" %(key, dat["value"]))
			if DATA[key].get("ifndef", False):
				fp.write("#endif // %s\n" %key)
		fp.write(FOOTER)


	def load_config(self):
		global DATA

		def set_default():
			for key,dat in DATA.iteritems():
				if not "value" in dat and "default" in dat:
					dat["value"] = dat["default"]

		try:
			fp = open("config.h")
		except (OSError, IOError):
			set_default()
			return
		match = re.compile('^[\t ]*#[\t ]*define[\t ]+([a-zA-Z0-9_]+)[\t ]*(.*)$')
		match2 = re.compile('^// ([a-zA-Z0-9_]+) is not set$')
		for line in fp:
			m = match.search(line)
			if m:
				m = m.groups()
				if not m[0] in DATA:
					continue
				if m[1] == "":
					DATA[m[0]]["value"] = True
				else:
					try:
						value = int(m[1])
					except ValueError:
						value = m[1]
					DATA[m[0]]["value"] = value
				if "encoding" in DATA[m[0]]:
					fun = getattr(field_encodings, DATA[m[0]]['encoding'])
					DATA[m[0]]["value"] = fun(DATA[m[0]]["value"], False)
			else:
				m = match2.search(line)
				if m and m.groups()[0] in DATA:
					m = m.groups()
					DATA[m[0]]["value"] = False

		set_default()

if __name__ == "__main__":
	App = OpenChronosApp()
	App.load_config()
	App.main()
	App.save_config()

