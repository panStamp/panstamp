/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var statement, statementType;

/**
 * Update values
 */
function updateValues()
{
  statement = window.parent.statement;
  statementType = window.parent.statementType;

  document.getElementById("item1").value = statement[1];
  document.getElementById("operator").value = statement[2];
  onchangeItem1();
}

/**
 * Item1 selected
 */
function onchangeItem1()
{
  statement[1] = document.getElementById("item1").value;

	switch(statement[1])
	{
		case "weekday":
			document.getElementById("item2box").src = "../edit_item2_wday.html";
			break;
		case "monthday":
			document.getElementById("item2box").src = "../edit_item2_mday.html";
			break;
		case "month":
			document.getElementById("item2box").src = "../edit_item2_month.html";
			break;
		case "time":
        if (statement[3].substring(0, 3) == "sun")
				  document.getElementById("item2box").src = "../edit_item2_sunl.html";
        else
          document.getElementById("item2box").src = "../edit_item2_time.html";
			break;
		case "date":
			document.getElementById("item2box").src = "../edit_item2_date.html";
			break;
		default:
			break;
	}
}

/**
 * Operator selected
 */
function onchangeOperator()
{
  statement[2] = document.getElementById("operator").value;

  var item2box = document.getElementById("item2box").contentWindow;
  var item2option = item2box.document.getElementById("item2option");
  if (item2option != null)
    item2box.changeOption();
}

/**
 * Return python representation of the time condition
 */
function getCondition()
{
  var pythonString = "clock." +  document.getElementById("item1").value + "() ";
  pythonString += document.getElementById("operator").value;

  var item2box = document.getElementById("item2box").contentWindow;
  pythonString += " " + item2box.getItem2();

  if (statementType == "trigger")
    pythonString = "clock.event and " + pythonString;

  return pythonString;
}

