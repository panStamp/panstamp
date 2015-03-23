/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

var newEvnid = "evn_00000001"

/**
 * Create values
 */
function updateValues()
{
  var jsonDoc = getJsonDoc();
  var events = jsonDoc.events;
  events.forEach(addEvent);
}

/**
 * Add event to the table
 */
function addEvent(event)
{
  var evntable = document.getElementById("evntable");
  
  var row = evntable.insertRow(evntable.rows.length);

  // Event name
  cell = row.insertCell(0);
  cell.className = "cellitem";
  evnname = document.createTextNode(event.name);
  cell.appendChild(evnname);

  // Actions
  cell = row.insertCell(1);
  cell.className = "cellaction";
  // Delete event
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "/command/delete_event?id=" + event.id);
  cfglink.onclick = function() {return confirm("Delete event?");};
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
  cfglink.setAttribute("href", "/edit_event.html?id=" + event.id);
  cell.appendChild(cfglink);
  img = document.createElement("img");
  img.setAttribute("src","/lagarto/images/edit.png");
  img.title = "edit";
  cfglink.appendChild(img);

  // New event id
  var intId = parseInt(event.id.substring(4));
  newEvnId = "" + (intId + 1);
  newEvnid = "evn_" + Array(9 - newEvnId.length).join("0") + newEvnId;
}


