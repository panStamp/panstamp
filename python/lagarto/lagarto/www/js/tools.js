/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Retrieve parameters from URL
 */
function getUrlVars()
{
  var vars = {};
  var parts = window.location.href.replace(/[?&]+([^=&]+)=([^&]*)/gi, function(m,key,value)
  {
    vars[key] = value;
  });

  return vars;
}

