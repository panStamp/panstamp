/* Copyright (c) Daniel Berenguer (panStamp) 2014 */

/**
 * Update table fields
 */
function updateValues()
{
  var moteAddress = getUrlVars()["address"];

  var jsonDoc = getJsonDoc();
  var swapnet = jsonDoc.network;

  swapnet.motes.forEach(function(mote)
  {
    if (mote.address == moteAddress)
    {
      document.getElementById("address").value = moteAddress;
      document.getElementById("newaddr").value = moteAddress;
      document.getElementById("txinterval").value = mote.txinterval;
      return;
    }
  });
}
