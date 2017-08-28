<?php

namespace ArangoDBClient;

// use the driver's autoloader to load classes
require '/home/seungchol/arangodb-php/autoload.php';
Autoloader::init();

// set up connection options
$connectionOptions = array(
  // endpoint to connect to
  ConnectionOptions::OPTION_ENDPOINT     => 'tcp://localhost:8529', 
  // can use Keep-Alive connection
  ConnectionOptions::OPTION_CONNECTION   => 'Keep-Alive',           
  // use basic authorization
  ConnectionOptions::OPTION_AUTH_TYPE    => 'Basic',                 
  // user for basic authorization
  ConnectionOptions::OPTION_AUTH_USER    => 'root',                      
  // password for basic authorization
  ConnectionOptions::OPTION_AUTH_PASSWD  => 'tmdcjf123',                      
  // timeout in seconds
  ConnectionOptions::OPTION_TIMEOUT      => 600,
  // database name 
  ConnectionOptions::OPTION_DATABASE     => '_system'
);
try {
  // establish connection
  $connection = new Connection($connectionOptions);
	$statement = new Statement(
		$connection, [
			'query' => 'FOR f IN FASTQ_LIST
				    FOR s IN SAM_LIST
				    FILTER f.USER_ID == @id && s.USER_ID == @id
				    FILTER f.IDX == s.IDX1 || f.IDX == reverse(s.IDX1)
				    RETURN f.FASTQ_COL',
			'bindVars' => [ 'id' => $argv[1] 
				      ]
			     ]);
	$cursor = $statement->execute();


} catch (ConnectException $e) {
  print $e . PHP_EOL;
} catch (ServerException $e) {
  print $e . PHP_EOL;
} catch (ClientException $e) {
  print $e . PHP_EOL;
}
