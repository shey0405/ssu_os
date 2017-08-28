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
				    FILTER f.USER_ID == @id && f.FASTQ_COL == @f_col
				    UPDATE f WITH {REFER_COL: @r_col, REFER_FILE_NAME: @r_fn} IN FASTQ_LIST',
			'bindVars' => [ 'id' => $argv[1], 'f_col' => $argv[4],
					'r_col' => $argv[3], 'r_fn' => $argv[2] 
				      ]
			     ]);
	$cursor = $statement->execute();

	$statement = new Statement(
		$connection, [
			'query' => 'FOR s IN SAM_LIST
				    FILTER s.USER_ID == @id && s.FILE_NAME == @file
				    UPDATE s WITH {FASTQ2_COL: @f_col, FASTQ2_FILE_NAME: @f_fn} IN SAM_LIST',
			'bindVars' => [ 'id' => $argv[1], 'file' => $argv[2],
					'f_col' => $argv[4], 'f_fn' => $argv[5]
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
