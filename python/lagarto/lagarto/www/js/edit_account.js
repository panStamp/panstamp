/* Copyright (c) Daniel Berenguer (panStamp) 2012 */

/**
 * Update table fields
 */
function updateValues()
{
  var params = getUrlVars();

  if ("user" in params)
  {
    var username = params["user"];
    document.getElementById("user").value = username;
    document.getElementById("olduser").value = username;
  }
}

/**
 * Process values before submiting them
 */
function processValues()
{
  var username = document.getElementById("user").value
  var olduser = document.getElementById("olduser").value
  var password = document.getElementById("password").value
  var confirm = document.getElementById("confirm").value

  if (password == "")
  {
    alert("Please enter a password");
    return;
  }
  if (password != confirm)
  {
    alert("Passwords don't match. Please re-type them again");
    return;
  }
  document.getElementById("password").value = MD5(password);
  document.getElementById("dataform").submit()
}

