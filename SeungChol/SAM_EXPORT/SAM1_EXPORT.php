<?php

namespace ArangoDBClient;

// use the driver's autoloader to load classes
require 'arangodb-php/autoload.php';
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
  ConnectionOptions::OPTION_TIMEOUT      => 30,
  // database name 
  ConnectionOptions::OPTION_DATABASE     => '_system'
);

try {
  // establish connection
  $connection = new Connection($connectionOptions);

  function export($collection, Connection $connection) {
    // settings to use for the export
    $settings = array(
      'batchSize' => 10000,  // export in chunks of 5K documents
      '_flat' => true,      // use simple PHP arrays
      'restrict' => array(
        'type' => 'include',
        'fields' => array('_key', 'QNAME', 'BITFLAG', 'RNAME', 'POS', 'MAPQ', 'CIGAR', 'RNEXT', 'PNEXT', 'TLEN', 'SEQ', 'QUAL', 'OPTION')
      )
    );

    $export = new Export($connection, $collection, $settings);

    // execute the export. this will return an export cursor
    $cursor = $export->execute();
    
    // now we can fetch the documents from the collection in batches
    while ($docs = $cursor->getNextBatch()) {
      $output = '';
      $cnt = count($docs);
      foreach ($docs as $doc) {
        $values = array(
	  $doc['_key'],
	  $doc['QNAME'],
	  $doc['BITFLAG'],
	  $doc['RNAME'],
	  $doc['POS'],
	  $doc['MAPQ'],
	  $doc['CIGAR'],
	  $doc['RNEXT'],
	  $doc['PNEXT'],
	  $doc['TLEN'],
	  $doc['SEQ'],
	  $doc['QUAL'],
          $doc['OPTION']
        );
        //$output .= '"' . implode('","', $values) . '"' . PHP_EOL;
	//$output .= implode(',', $values) . PHP_EOL;
	//$output .= implode(PHP_EOL, $values) . PHP_EOL;
	//$output .= $values[0] . PHP_EOL . $values[1] . PHP_EOL . $values[2] . PHP_EOL . '+' . PHP_EOL . $values[3] . PHP_EOL;
	//$output .= str_replace('|', '	', $values[2]) . PHP_EOL;
	$values[12] = str_replace('|', '	', $values[12]);
	$output .= implode('	', $values) . PHP_EOL;
      }

      // print out the data directly 
      print $output;
    }
  }

  // run the export
  export('sam1', $connection);

} catch (ConnectException $e) {
  print $e . PHP_EOL;
} catch (ServerException $e) {
  print $e . PHP_EOL;
} catch (ClientException $e) {
  print $e . PHP_EOL;
}
