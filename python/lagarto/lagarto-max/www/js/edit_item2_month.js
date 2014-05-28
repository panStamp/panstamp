/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update values
 */
function updateValues()
{
  var item2 = window.parent.statement[3];
  var intM = monthToInt(item2);

  document.getElementById("month").value = intM;
}

/**
 * Return python representation of item2
 */
function getItem2()
{
  var month = document.getElementById("month").value;

  return month;
}

