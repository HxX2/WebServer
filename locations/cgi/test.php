<?php
	$content = "This is the content of the response.";
	header("Status: " . "200 OK");
	header("Content-Length: " . strlen($content));
	echo $content;
?>
