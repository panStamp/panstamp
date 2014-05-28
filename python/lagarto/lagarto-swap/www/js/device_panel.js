/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update table fields
 */
function updateValues()
{
  var jsonDoc = getJsonDoc();
  var swapnet = jsonDoc.network;
  swapnet.motes.forEach(addMote);
}

/**
 * Add mote
 */
function addMote(mote)
{
  var nettable = document.getElementById("nettable");
  var row, cell, label, cfglink, img;

  row = nettable.insertRow(nettable.rows.length);

  // Address
  cell = row.insertCell(0);
  cell.className = "cellitem";
  label = document.createTextNode(mote.address);
  cell.appendChild(label);
  // Developer
  cell = row.insertCell(1);
  cell.className = "cellitem";
  label = document.createTextNode(mote.manufacturer);
  cell.appendChild(label);
  // Description
  cell = row.insertCell(2);
  cell.className = "cellitem";
  label = document.createTextNode(mote.name);
  cell.appendChild(label);
  // Action
  cell = row.insertCell(3);
  cell.className = "cellaction";
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "/command/delete_mote/?address=" + mote.address);
  cfglink.onclick = function() {return confirm("Delete mote?");};
  cfglink.setAttribute("alt", "delete");
  cfglink.style.pointer = "cursor";
  cell.appendChild(cfglink);
  img = document.createElement("img");
  img.setAttribute("src","/lagarto/images/delete.png");
  img.title = "delete";
  cfglink.appendChild(img);
}

