/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update values
 */
function updateValues()
{
  var item2 = window.parent.statement[3];
  document.getElementById("item2").value = item2;
}

/**
 *  Switch item2 to network type
 */
function switchItem2()
{
  if (document.getElementById("netopt").checked)
	  window.parent.document.getElementById("item2box").src = "../edit_item2_network.html";
}

/**
 * Return python representation of item2
 */
function getItem2()
{
  var itemtype = document.getElementById("itemtype").value;

  if (itemtype == "state")
    return "\"" + document.getElementById("item2").value + "\"";

  return document.getElementById("item2").value;
}

