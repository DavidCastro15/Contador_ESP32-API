<?php

try {

    //$user = $_GET['user'] ;
    //$camion =  $_GET['camion'] ;
    $bultos = $_GET['bultos'] ;

    require_once ('model/config.php');


   
    $id = $bd->query("SELECT * FROM conteo ORDER BY id DESC LIMIT 1")->fetchAll(PDO::FETCH_OBJ);
    $id = $id[0]->id;

    $sql = "UPDATE conteo SET  bultos_contados = '$bultos' where id = '$id';";

    $res = $bd->prepare($sql)->execute();

    if($res == 1){
        echo 'Insertado';
    }

  
    
    
} catch (Exception $e) {
    echo $e->getMessage();
}


//  1-Tener identificado la funcion principal del contador
//  2-Identificar la funcion para obtener el valor de los bultos a contar
//  3-Asignar el value a la variable del arduino
//  4-Identificar la funcion para actualizar el registro de los bultos contados por el modulo
//  5-Pausar el loop asta obtener el siguiente numero de bultos de la web



