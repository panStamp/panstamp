/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var updateIntervals = {"oc" : "on change",
                       "1m" : "1 minute",
                       "5m" : "5 minutes",
                       "15m" : "15 minutes",
                       "30m" : "30 minutes",
                       "1h" : "1 hour",
                       "12h" : "12 hours",
                       "1d" : "1 day",
                       "1w" : "1 week",
                       "1M" : "1 month"};

/**
 * Create values
 */
function updateValues()
{
  var jsonDoc = getJsonDoc();
  var database = jsonDoc.database;

  for(var tableName in database)
    addTable(tableName, database[tableName]);
}

/**
 * Add table
 */
function addTable(tableName, table)
{
  var dbtable = document.getElementById("dbtable");
  var row = dbtable.insertRow(dbtable.rows.length);

  // Table name
  cell = row.insertCell(0);
  cell.className = "cellitem";
  tName = document.createTextNode(tableName);
  cell.appendChild(tName);

  // Update interval
  cell = row.insertCell(1);
  cell.className = "cellitem";
  tableInterval = document.createTextNode(updateIntervals[table.interval]);
  cell.appendChild(tableInterval);

  // Actions
  cell = row.insertCell(2);
  cell.className = "cellaction";
  // Delete table
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "/command/delete_db_table/?name=" + tableName);
  cfglink.onclick = function() {return confirm("Delete table?");};
  cfglink.setAttribute("alt", "delete");
  cfglink.style.pointer = "cursor";
  cell.appendChild(cfglink);
  img = document.createElement("img");
  img.setAttribute("src","/lagarto/images/delete.png");
  img.title = "delete";
  cfglink.appendChild(img);
  // Edit event
  cell.className = "cellaction";
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "/edit_db_table.html/?name=" + tableName);
  cell.appendChild(cfglink);
  img = document.createElement("img");
  img.setAttribute("src","/lagarto/images/edit.png");
  img.title = "edit";
  cfglink.appendChild(img);
}


