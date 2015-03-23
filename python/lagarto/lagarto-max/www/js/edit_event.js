/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var eventId = getUrlVars()["id"];

if ("linenb" in getUrlVars())
  window.location = window.location.protocol + '//' + window.location.hostname + ":" + window.location.port + "/edit_event.html?id=" + eventId;

/**
 * Create table fields
 */
function createValues()
{
  loadJSONdata("/command/get_event?id=" + eventId, updateValues);
}

/**
 * Update table values
 */
function updateValues()
{
  var jsonDoc = getJsonDoc();
  var event = jsonDoc.event;
  var lineNb = 0, start, end;
  var commentCnt = 0;

  document.getElementById("evnid").value = event.id;
  document.getElementById("evnname").value = event.name;

  event.code.forEach(function(line)
  {
    lineNb++;

    if (line == '"""')
      commentCnt++;
    else if (line.substring(0, 5) === "elif ")
    {
      start = 5;
      if (line.substring(start, 21) === "clock.event and ")
        start = 21;
      end = line.indexOf(":", start);
      if (end > -1)
        createTrigger(line.substring(start, end), lineNb);
    }
    else if (line.substring(0, 8) === "if not (")
    {
      start = 8;
      end = line.indexOf("):", start);
      if (end > -1)
        createCondition(line.substring(start, end), lineNb);
    }
    else if (line != "return" && line != "pass" && line != "if False:" && line != "else:" && commentCnt > 1)
      createAction(line, lineNb);
  });
}

/**
 * Create trigger
 */
function createTrigger(line,lineNb)
{
  trigtable = document.getElementById("trigtable");
  condition = conditionToWeb(line);
  insertRow(trigtable, condition, lineNb, "trigger");
}

/**
 * Create condition
 */
function createCondition(line,lineNb)
{
  condtable = document.getElementById("condtable");
  condition = conditionToWeb(line);
  insertRow(condtable, condition, lineNb, "condition");
}

/**
 * Create action
 */
function createAction(line,lineNb)
{
  acttable = document.getElementById("acttable");
  action = actionToWeb(line);
  insertRow(acttable, action, lineNb, "action");
}

/**
 * Insert row in table
 */
function insertRow(table, line, lineNb, statement)
{
  var row = table.insertRow(table.rows.length);

  // Statement
  cell = row.insertCell(0);
  cell.className = "cellitem";
  stat = document.createTextNode(line[1] + " " + line[2] + " " + line[3]);
  cell.appendChild(stat);

  // Actions
  cell = row.insertCell(1);
  cell.className = "cellaction";
  // Delete statement
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "/command/delete_event_line?id=" + eventId + "&linenb=" + lineNb);
  cfglink.onclick = function() {return confirm("Delete line?");};
  cfglink.setAttribute("alt", "delete");
  cfglink.style.pointer = "cursor";
  cell.appendChild(cfglink);
  img = document.createElement("img");
  img.setAttribute("src","/lagarto/images/delete.png");
  img.title = "delete";
  cfglink.appendChild(img);
  // Edit event
  cell = row.insertCell(2);
  cell.className = "cellaction";
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "/edit_" + statement + ".html?id=" + eventId + "&linenb=" + lineNb);
  cell.appendChild(cfglink);
  img = document.createElement("img");
  img.setAttribute("src","/lagarto/images/edit.png");
  img.title = "edit";
  cfglink.appendChild(img);
}

/**
 * Create new trigger
 */
function newTrigger()
{
  window.location.href = "/edit_trigger.html?id=" + eventId + "&linenb=0";
}

/**
 * Create new condition
 */
function newCondition()
{
  window.location.href = "/edit_condition.html?id=" + eventId + "&linenb=0";
}

/**
 * Create new action
 */
function newAction()
{
  window.location.href = "/edit_action.html?id=" + eventId + "&linenb=0";
}

