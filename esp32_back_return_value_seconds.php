<?php

try {

    require_once ('model/config.php');

    $bultos = $bd->query("SELECT * FROM conteo ORDER BY id DESC LIMIT 1")->fetchAll(PDO::FETCH_OBJ);
    $camion = $bultos[0]->camion;
    $bultos1 = $bultos[0]->bultos;
    $c_bultos = $bultos[0]->bultos_contados;
    $arr = array('camion'=>$camion,'bultos'=>$bultos1,'bultos_contados'=>$c_bultos);
    echo json_encode($arr);
    
    
} catch (Exception $e) {
    echo $e->getMessage();
}

//  1-Tener identificado la funcion principal del contador
//  2-Identificar la funcion para obtener el valor de los bultos a contar
//  3-Asignar el value a la variable del arduino
//  4-Identificar la funcion para actualizar el registro de los bultos contados por el modulo
//  5-Pausar el loop asta obtener el siguiente numero de bultos de la web


