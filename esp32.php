<?php
session_start();
include_once 'model/config.php';
$auth = $_SESSION['login'];

if (!$auth) {
    header('Location: index.php?mensaje=error');
}

?>
<!DOCTYPE html>
<html lang="en">
<?php include 'utilidades/menu.php'; ?>

<body>
    <div class="container" style="margin-top: 2rem;">
        <div class="card">
            <div class="card-header">
                <strong> Conteo de bultos</strong>
            </div>
            <div class="card-body">
                <form action="esp32_back.php" method="POST">
                    <div class="mb-3">
                        <label for="Camion" class="form-label">Camion</label>
                        <input type="text" class="form-control" name="camion">
                    </div>
                    <div class="mb-3">
                        <label for="Bultos" class="form-label">Bultos</label>
                        <input type="text" class="form-control" name="bultos">
                    </div>
                    <button type="submit" class="btn btn-primary">Enviar</button>
                </form>
            </div>
        </div>
    </div>
    <div class="container" style="margin-top: 2rem;">
        <div class="card text-center">
            <div class="card-header ">
                <div class="alert alert-success" id="vacio2" role="alert">
                    Estado actual del contador : Contando
                    <div class="spinner-border ms-auto text-success" role="status" aria-hidden="true"></div>
                </div>
            </div>
            <div class="card-body">
                <table class="table">
                    <thead>
                        <tr>
                            <th scope="col">Camion</th>
                            <th scope="col">Bultos a contar</th>
                            <th scope="col">Bultos Contando(Tiempo real)</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr>
                            <td><input class="form-control text-center" readonly id="camion" type="text"></td>
                            <td><input class="form-control text-center" readonly id="bultos" type="text"></td>
                            <td><input class="form-control text-center" readonly id="bultosContando" type="text"></td>
                        </tr>
                    </tbody>
                </table>
            </div>
            <div class="card-footer">
                <div class="alert alert-warning" id="vacio" role="alert">
                    Esperando la siguiente carga de bultos
                    <div class="spinner-border ms-auto text-warning" role="status" aria-hidden="true"></div>
                </div>
            </div>
        </div>
    </div>
    <script>
        history.replaceState("http://localhost/Aplics/esp32.php", "Aplics", "esp32.php");

        function cambiar() {
            $.get("esp32_back_return_value_seconds.php",
                function(data) {
                    if (data == '') {} else {
                        const obj = JSON.parse(data);
                        $("#camion").val(obj.camion);
                        $("#bultos").val(obj.bultos);
                        $("#bultosContando").val(obj.bultos_contados);
                        document.getElementById("vacio").style.display = "none";
                        document.getElementById("vacio2").style.display = "block";
                        document.getElementById("camion").style.display = "block";
                        document.getElementById("bultos").style.display = "block";
                        document.getElementById("bultosContando").style.display = "block";
                        if (obj.bultos_contados == '') {
                            document.getElementById("vacio").style.display = "block";
                            document.getElementById("vacio2").style.display = "none";


                        }
                        if (obj.bultos_contados == obj.bultos) {
                            document.getElementById("camion").style.display = "none";
                            document.getElementById("bultos").style.display = "none";
                            document.getElementById("bultosContando").style.display = "none";
                            document.getElementById("vacio2").style.display = "none";
                            document.getElementById("vacio").style.display = "block";
                        }

                    }
                });
        }
        setInterval(() => cambiar(), 1000)
    </script>
</body>
<?php include 'utilidades/footer.php'; ?>

</html>