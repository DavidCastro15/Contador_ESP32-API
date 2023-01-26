<?php

try {

    $user = $_GET['user'] ;
    /* $camion =  $_GET['camion'] ;
    $bultos =  $_GET['bultos'] ; */

    require_once ('model/config.php');

   
    $estatus = $bd->query("SELECT * FROM conteo ORDER BY id DESC LIMIT 1")->fetchAll(PDO::FETCH_OBJ);
    $estatus = $estatus[0]->estatus;
    echo $estatus;
    
    
} catch (Exception $e) {
    echo $e->getMessage();
}
