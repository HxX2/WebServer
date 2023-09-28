<?php
	$content = phpinfo();
	header("Content-Length: " . strlen($content));
	echo $content;
?>
