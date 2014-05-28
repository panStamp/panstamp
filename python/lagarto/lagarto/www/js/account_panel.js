/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update table fields
 */
function updateValues()
{
  var jsonDoc = getJsonDoc();
  var accounts;

  accounts = jsonDoc.accounts;
  accounts.forEach(addAccount);
}

/**
 * Add account to the table
 */
function addAccount(user)
{
  var acctable = document.getElementById("acctable");
  var row = acctable.insertRow(acctable.rows.length);
  var cell, cfglink, img;

  // User name
  cell = row.insertCell(0);
  cell.className = "cellitem";
  label = document.createTextNode(user);
  cell.appendChild(label);
  // Delete option
  cell = row.insertCell(1);
  cell.className = "cellaction";
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "/core/delete_account/?user=" + user);
  cfglink.onclick = function() {return confirm("Delete user?");};
  cfglink.setAttribute("alt", "delete");
  cfglink.style.pointer = "cursor";
  cell.appendChild(cfglink);
  img = document.createElement("img");
  img.setAttribute("src","/lagarto/images/delete.png");
  img.title = "delete";
  cfglink.appendChild(img);
  // Edit option
  cell = row.insertCell(2);
  cell.className = "cellaction";
  cfglink = document.createElement("a");
  cfglink.setAttribute("href", "/lagarto/edit_account.html/?user=" + user);
  cell.appendChild(cfglink);
  img = document.createElement("img");
  img.setAttribute("src","/lagarto/images/edit.png");
  img.title = "edit";
  cfglink.appendChild(img);
}

/**
 * Delete account
 */
function deleteAccount(username)
{
  return confirm("Delete " + username + "?");
}
