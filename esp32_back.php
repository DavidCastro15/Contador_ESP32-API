<?php

try {

    $camion = isset($_POST['camion']) ? $_POST['camion'] : null;
    $bultos = isset($_POST['bultos']) ? $_POST['bultos'] : null;
    $estatus = 1;

    require_once ('model/config.php');

    $sql = "INSERT INTO conteo(camion, bultos,estatus) VALUES (?,?,?)";
    $res = $bd->prepare($sql)->execute(array($camion, $bultos, $estatus));
    
    if ($res === true) {
        header('location: esp32.php?mensaje=registrado');
    } else {
        header('location: esp32.php?mensaje=error');
    }
   
} catch (Exception $e) {
    echo $e->getMessage();
}
