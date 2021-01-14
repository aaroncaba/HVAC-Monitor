<?php
// Use as:  
// http://hvac-monitor/addReading.php?signal_id=9&state=off

$hostname = "hvac-monitor";
$username = "arduino";
$password = "nml0vOeQW3WmzxqY";
$db = "hvac";

$dbconnect = mysqli_connect($hostname, $username, $password, $db);

if ($dbconnect->connect_error) {
	die("Database connection failed: " . $dbconnect->connect_error);
}

$device = $_GET['device'];
$command = $_GET['command'];
$state = $_GET['state'];

// $query = "INSERT INTO states (signal_id, state) VALUES ('$signal_id', '$state')";

$query = "
INSERT INTO `states`(`signal_id`, `state`)
VALUES(
    (
    SELECT
        s.`id`
    FROM
        `signals` s
    INNER JOIN `devices` d INNER JOIN `device_commands` dc ON
        (
            s.`device_id` = d.`id` AND s.`command_id` = dc.`id`
        )
    WHERE
        d.`name` = '$device' AND dc.`command` = '$command'
	),
	'$state'
);";

echo $query;
echo "\n";

if (!mysqli_query($dbconnect, $query)) {
	die('An error occurred when submitting your review.');
} else {
	echo "Thanks for your review.";
}
