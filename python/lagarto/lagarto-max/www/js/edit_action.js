/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var eventId, lineNb, statement, codeLine = "";

/**
 * Create table fields
 */
function createValues()
{
  eventId = getUrlVars()["id"];
  lineNb = getUrlVars()["linenb"];

  document.getElementById("evnid").value = eventId;
  document.getElementById("linenb").value = lineNb;

  statement = ["network", "", "=" , ""];
  loadJSONdata("/command/get_event/?id=" + eventId, updateValues);   
  document.getElementById("source").value = statement[0];
}

/**
 * Update values
 */
function updateValues()
{
  var jsonDoc = getJsonDoc();
  var event = jsonDoc.event;

  document.getElementById("evnname").value = event.name;

  if (lineNb > 0)
  {
    codeLine = event.code[lineNb-1];
    statement = actionToWeb(codeLine);
  }

  document.getElementById("source").value = statement[0];
  onchangeOption();
}

/**
 * Type of action selected
 */
function onchangeOption()
{
	var option = document.getElementById("source").value;

	switch(option)
	{
		case "network":
      if (statement[0] != "network")
        statement = ["network", "", "=" , ""];
			document.getElementById("itemeditor").src = "/edit_act_network.html";
      break;
		case "cloud":
      if (statement[0] != "cloud")
        statement = ["cloud", "", "=" , ""];
			document.getElementById("itemeditor").src = "/edit_act_cloud.html";
			break;
    default:
      break;
  }
  statement[0] = option;
}

/**
 * Submit python string to server
 */
function submitForm()
{
  var editor = document.getElementById("itemeditor").contentWindow;
  var act = editor.getAction();

  if (act != null)
  {
    document.getElementById("line").value = act;
    document.getElementById("python_statement").submit();
  }
}

