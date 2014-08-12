# vim: ts=4 noexpandtab
# encoding: utf-8
#
#		   Copyright (C) 2012 Angelo Arrifano <miknix@gmail.com>
#
# This file is part of OpenChronos. This file is free software: you can
# redistribute it and/or modify it under the terms of the GNU General Public
# License as published by the Free Software Foundation, version 2.
#
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
# FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
# details.
#
# You should have received a copy of the GNU General Public License along with
# this program; if not, write to the Free Software Foundation, Inc., 51
# Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
#

import os
import ConfigParser

def get_modules():
	mods = os.listdir('modules/')
	mods = filter( (lambda x: x[-2:] == '.c'), mods)
	mods = map( (lambda x: x[:-2]), mods)
	return mods

def read_config():
	DATA = []
	mods = get_modules()
	for mod in mods:
		cfgname = "modules/%s.cfg" % (mod)
		cfg = ConfigParser.RawConfigParser()
		cfg.read(cfgname)
		parent = mod.upper()
		sectNr = 0
		for section in cfg.sections():
			try:
				name = cfg.get(section, 'name')
				help = cfg.get(section, 'help')
			except ConfigParser.NoOptionError:
				print "%s:%s: Error: name, help are mandatory!" % (cfgname, section)
				continue

			item = {
				'name'   : name,
				'help'   : help,
				'ischild': (sectNr > 0)
			}

			opt =  {'type': "bool", 'default': "", 'depends': "", 'encoding': None}
			for key,default in opt.iteritems():
				try:
					item[key] = cfg.get(section, key)
				except ConfigParser.NoOptionError:
					if default != None:
						item[key] = default

			# build dependency array from string
			item['depends'] = filter(None, map(lambda x: x.strip(), item['depends'].split(',')))
			if sectNr > 0: item['depends'].append( "CONFIG_MOD_%s" % (parent) )

			# Special treatment for booleans
			if item['type'] == "bool":
				item['default'] = bool(item['default'])

			DATA.append( ("CONFIG_MOD_%s" % (section), item) )
			if sectNr == 0 and section != parent:
				print "%s: Warn: The [%s] section must be the first!" \
						% (cfgname, item['parent'])
			if not section.startswith(parent):
				print "%s:%s: Warn: section name should have prefix '%s_'" \
						% (cfgname, section, parent)

			sectNr += 1

	return DATA
