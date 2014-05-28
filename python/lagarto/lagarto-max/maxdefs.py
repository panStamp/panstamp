#########################################################################
#
# maxdefs
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
# panStamp is distributed in the hope that it will be useful,
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
__date__  ="$Oct 23, 2012$"
#########################################################################

import os

    
class MaxDefinitions:
    """
    Global definitions across all lagarto-max classes and modules
    """
    working_dir = os.path.dirname(__file__)
    _tmp_dir = os.path.split(working_dir)[0]
    lagarto_dir = os.path.join(_tmp_dir, "lagarto")
    ip_address = ""

