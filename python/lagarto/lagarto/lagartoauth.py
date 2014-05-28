#########################################################################
#
# Copyright (c) 2012 Daniel Berenguer <dberenguer@usapiens.com>
#
# This file is part of the lagarto project.
#
# lagarto  is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
# lagarto is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with panLoader; if not, write to the Free Software
# Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301
# USA
#
#########################################################################
__author__="Daniel Berenguer"
__date__  ="$Feb 17, 2012$"
#########################################################################

import json
import os
from barrel import cooper
from barrel.basic import BasicAuth
import hashlib


class LagartoAuth(BasicAuth):
    """
    Lagarto HTTP authentication class
    """

    ## Path to the HTTP authentication file
    secu_dir = os.path.join(os.path.dirname(__file__), "security")
    ## Path to the HTTP authentication file
    httpauth = os.path.join(secu_dir, "httpauth")   

    
    def valid_user(self, username, password):
        """
        Is this a valid username/password? (True or False)
        Method overriden
        
        @param username: user name
        @param password: associated password
        
        @return True if the supplied username/password pair is valid. Return
        False otherwise
        """
        return self._check_account(username, password)


    def _check_account(self, username, password):
        """
        Check if username/password pair is valid or not
        
        @param username: user name
        @param password: associated password
        
        @return True if username/password is valid. False otherwise
        """
        # Encript password
        crypted_password = hashlib.md5(password).hexdigest()
        
        try:
            # Open HTTP authentication file
            f = open(LagartoAuth.httpauth)
            # Load json data
            secu_data = json.load(f)["accounts"]
            f.close()
            
            if username not in secu_data:
                return False
            
            if secu_data[username] != crypted_password:
                return False
            return True
        except IOError:
            return False


    @staticmethod
    def set_account(username, password, oldusername=None):
        """
        Set user account in HTTP auth file
        
        @param username: user name
        @param password: associated password, crypted in MD5 format
        @param oldusername: old user name, in case the current one has changed
        
        @return True if the user account is successfully set in http auth file
        """       
        try:
            # Open HTTP authentication file
            f = open(LagartoAuth.httpauth, 'r')
            # Load json data
            secu_data = json.load(f)
            f.close()

            # Delete old username
            if oldusername is not None:
                if oldusername != username:
                    del secu_data["accounts"][oldusername]
                
        except IOError:
            secu_data = {}
                
        if "accounts" not in secu_data:
            secu_data["accounts"] = {}

        # Update login account
        secu_data["accounts"][username] = password
        
        try:
            # Save data in HTTP auth file
            f = open(LagartoAuth.httpauth, 'w')
            json.dump(secu_data, f, sort_keys=False, indent=2)           
            f.close()
            return True
        except IOError:
            return False
        

    @staticmethod
    def delete_account(username):
        """
        Delete user account from HTTP auth file
        
        @param username: user name
        
        @return True if the user account is successfully removed from the http auth file
        """
        result = False
        
        try:
            # Open HTTP authentication file
            f = open(LagartoAuth.httpauth, 'r')
            # Load json data
            secu_data = json.load(f)
            f.close()
            
            # Locate account
            if username in secu_data["accounts"]:           
                # Remove account
                del secu_data["accounts"][username]
                result = True
            
            # Save data in HTTP auth file
            f = open(LagartoAuth.httpauth, 'w')
            json.dump(secu_data, f, sort_keys=False, indent=2)           
            f.close()
            return result
        except IOError:
            return False


    @staticmethod
    def get_user_names():
        """
        Get user names from HTTP auth file
        
        @return list of user names
        """     
        try:
             # Open HTTP authentication file
            f = open(LagartoAuth.httpauth)
            # Load json data
            secu_data = json.load(f)
            f.close()
  
            usernames = []
            if "accounts" in secu_data:
                for user in secu_data["accounts"].keys():
                    usernames.append(user)

            return usernames
        except IOError:
            return []
        
        
    @staticmethod
    def is_security_enabled():
        """
        Check whether HTTP auth is enabled or not
        
        @return True if HTTP basic authentication is enabled. Return False otherwise
        """
        try:
            f = open(LagartoAuth.httpauth)
            secu_data = json.load(f) 
            f.close()
            
            if "accounts" not in secu_data:
                return False
            elif len(secu_data["accounts"]) == 0:
                return False
            return True
        except IOError:
            return False

        
    def __init__(self, app):
        """
        Constructor
        
        @param app: http server process
        """
        BasicAuth.__init__(self, app, users=None)


# Decorators
def auth_disabled(func):
    """
    This decorator doesn't add any behavior
    """
    return func


auth_enabled = cooper.decorize(LagartoAuth)

lagartoauth = auth_enabled() if LagartoAuth.is_security_enabled() else auth_disabled
