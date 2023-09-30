<?php

if (!isset($_SERVER['HTTP_COOKIE']))
{
	echo "No cookies found.";
	setcookie("Auction_Item_1", "Luxury Car", time() + 2 * 24 * 60 * 60);
}
else
	echo $_SERVER['HTTP_COOKIE'];

?>