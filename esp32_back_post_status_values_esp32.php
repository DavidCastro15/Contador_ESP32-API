<?php
try {

//$user = $_GET['user'] ;
//$camion =  $_GET['camion'] ;
$status = $_GET['status'] ;

require_once ('model/config.php');

$status = 0;


$id = $bd->query("SELECT * FROM conteo ORDER BY id DESC LIMIT 1")->fetchAll(PDO::FETCH_OBJ);
$id = $id[0]->id;

$sql = "UPDATE conteo SET  estatus = '$status' where id = '$id';";

$res = $bd->prepare($sql)->execute();




} catch (Exception $e) {
echo $e->getMessage();
}

?>