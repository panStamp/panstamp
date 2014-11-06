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
  var cloud = jsonDoc.cloud;

  for(var txName in cloud)
    addTransmission(txName, cloud[txName]);
}

/**
 * Add transmission
 */
function addTransmission(txName, transmission)
{
  var dbtable = document.getElementById("cloudtable");
  var row = dbtable.insertRow(dbtable.rows.length);

  // Name of transmission
  cell = row.insertCell(0);
  cell.className = "cellitem";
  tName = document.createTextNode(txName);
  cell.appendChild(tName);

  // Update interval
  cell = row.insertCell(1);
  cell.className = "cellitem";
  txInterval = document.createTextNode(updateIntervals[transmission.interval]);
  cell.appendChild(txInterval);

  // Cloud service
  cell = row.insertCell(2);
  cell.className = "cellitem";
  service = document.createTextNode(transmission.service);
  cell.appendChild(service);
  
  // Actions
  cell = row.insertCell(3);
  cell.className = "cellaction";
  // Delete transmission
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "/command/delete_cloud_tx/?name=" + txName);
  cfglink.onclick = function() {return confirm("Delete transmission?");};
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
  cfglink.setAttribute("href", "/edit_cloud_tx.html/?name=" + txName);
  cell.appendChild(cfglink);
  img = document.createElement("img");
  img.setAttribute("src","/lagarto/images/edit.png");
  img.title = "edit";
  cfglink.appendChild(img);
}
