<?php
	$content = phpinfo();
	header("Status: " . "200 OK");
	header("Content-Length: " . strlen($content));
	echo $content;
?>
