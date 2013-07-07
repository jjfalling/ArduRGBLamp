<?php
/*
#****************************************************************************
#*   process.php                                                            *
#*   Gets a rgb hex value and converts it for an arduino                    *
#*                                                                          *
#*   Copyright (C) 2013 by Jeremy Falling except where noted.               *
#*                                                                          *
#*   This program is free software: you can redistribute it and/or modify   *
#*   it under the terms of the GNU General Public License as published by   *
#*   the Free Software Foundation, either version 3 of the License, or      *
#*   (at your option) any later version.                                    *
#*                                                                          *
#*   This program is distributed in the hope that it will be useful,        *
#*   but WITHOUT ANY WARRANTY; without even the implied warranty of         *
#*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          *
#*   GNU General Public License for more details.                           *
#*                                                                          *
#*   You should have received a copy of the GNU General Public License      *
#*   along with this program.  If not, see <http://www.gnu.org/licenses/>.  *
#****************************************************************************
*/


//CHANGE THIS to the hostname or ip of the arduino
$arduinoAddress = "arduino.hostname";


$stringData=htmlspecialchars($_GET["color"]);

//get the individual colors
list($red, $green, $blue) = str_split($stringData, 2);

//convert from hex to decimal
$red = hexdec($red);
$green = hexdec($green);
$blue = hexdec($blue);

echo "$red $green $blue";

//my led is a common anode led. if yours is not, comment out the following 3 lines or your colors will be inverted or off
$red = 255 - $red;
$blue = 255 - $blue;
$green = 255 - $green;

//make http request to arduino
file_get_contents('http://' . $arduinoAddress . '/lamp?r=' . $red . '&g=' . $green . '&b=' . $blue . '');



?>