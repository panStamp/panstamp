/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update values
 */
function updateValues()
{
  var item2 = window.parent.statement[3];
  document.getElementById("suntime").value = item2;
}

/**
 * Return python representation of item2
 */
function getItem2()
{
  var item2 = document.getElementById("suntime").value;
  return "clock." + item2 + "()";
}

/**
 * Switch to sunlight time
 */
function switchItem2()
{
	window.parent.document.getElementById("item2box").src = "../edit_item2_time.html";
}

