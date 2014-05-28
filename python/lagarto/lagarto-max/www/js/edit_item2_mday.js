/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update values
 */
function updateValues()
{
  var item2 = window.parent.statement[3];
  
  fillDay(item2);
}

/**
 * Fill month day field
 */
function fillDay(item2)
{
	var fldDay = document.getElementById("mday");

	for(i=1 ; i<=31 ; i++)
		fldDay.options[i] = new Option(i, i);

  fldDay.value = item2;
}

/**
 * Return python representation of item2
 */
function getItem2()
{
  return document.getElementById("mday").value;
}

