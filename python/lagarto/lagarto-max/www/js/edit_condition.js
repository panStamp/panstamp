/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var eventId, lineNb, statement, statementType = "trigger";

/**
 * Create table fields
 */
function createValues()
{
  eventId = getUrlVars()["id"];
  lineNb = getUrlVars()["linenb"];

  document.getElementById("evnid").value = eventId;
  document.getElementById("linenb").value = lineNb;
  statementType = document.getElementById("type").value;

  statement = ["clock", "time", "==" , "00:00"];
  loadJSONdata("/command/get_event?id=" + eventId, updateValues);  

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
    var line = event.code[lineNb-1];
    var start, end;

    if (line.substring(0, 5) === "elif ")
    {
      start = 5;
      if (line.substring(5, 21) === "clock.event and ")
        start = 21;

      end = line.indexOf(":", start);
      if (end > -1)
        statement = conditionToWeb(line.substring(start, end));
    }
    else if (line.substring(0, 8) === "if not (")
    {
      start = 8;
      end = line.indexOf("):", start);
      if (end > -1)
        statement = conditionToWeb(line.substring(start, end));
    }
  }

  document.getElementById("source").value = statement[0];
  onchangeOption();
}

/**
 * Type of trigger selected
 */
function onchangeOption()
{
	var option = document.getElementById("source").value;

	switch(option)
	{
		case "clock":
      if (statement[0] != "clock")
        statement = ["clock", "time", "==" , "00:00"];
			document.getElementById("itemeditor").src = "/edit_cond_clock.html";
			break;
		case "network":
      if (statement[0] != "network")
        statement = ["network", "", "==" , ""];
			document.getElementById("itemeditor").src = "/edit_cond_network.html";
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
  var cond = editor.getCondition();

  if (cond != null)
  {
    document.getElementById("line").value = cond; 
    document.getElementById("python_statement").submit();
  }
}

