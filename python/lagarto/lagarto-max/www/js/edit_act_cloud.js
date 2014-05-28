/* Copyright (c) Daniel Berenguer (panStamp) 2014 */

var codeLine, statement, statementType = "action";
var endpointTypes = {};

/**
 * Update values
 */
function updateValues()
{
  codeLine = window.parent.codeLine;
  statement = window.parent.statement;

  document.getElementById("service").value = statement[1];
  onchangeService();
}

/**
 * Cloud service selected
 */
function onchangeService()
{
  var service = document.getElementById("service").value;

  if (service == "pachube")
	  document.getElementById("item2box").src = "../edit_item2_pachube.html";
  else if (service == "thingspeak")
    document.getElementById("item2box").src = "../edit_item2_thingspeak.html";
  else if (service == "opensense")
    document.getElementById("item2box").src = "../edit_item2_opensense.html";
  else if (service == "twitter")
    document.getElementById("item2box").src = "../edit_item2_twitter.html";
  else if (service == "autoremote")
    document.getElementById("item2box").src = "../edit_item2_autoremote.html";
  else if (service == "grovestreams")
    document.getElementById("item2box").src = "../edit_item2_grovestreams.html";
}

/**
 * Return python representation of the network action
 */
function getAction()
{
  var item1 = document.getElementById("service").value;

  var item2box = document.getElementById("item2box").contentWindow;
  var item2 = item2box.getItem2();

  if (item2 == null)
    return null;
  var pythonString = "cloud.push_" + item1 + "(" + item2 + ")";

  return pythonString;
}

