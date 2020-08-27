
<?php

//accedemos al servidor
$conn = mysqli_connect("iotdarkmaxvs.ga","admin_iotdarkDB","Q1w2e3r4t5","admin_iotdarkDB");

if ($conn==false){
    echo "Hubo un problema al conectarse a María DB";
    die();
  }

$result = $conn->query("SELECT * FROM temps");
//$dataDB = $result->fetch_all(MYSQLI_ASSOC);
//$numFilas = count($dataDB);

/* a continuación crearemos un array donde cargaremos todos los datos 
que deseemos mostrar en la gráfica */
/* la primera posición del array la reservamos a los nombres de las métricas, 
en nuestro caso serán dos métricas: Distancia y fecha */

$i=0;
/*creamos un bucle que recorrerá todas las filas generadas en nuestra consulta,
 y por cada fila se almacenará en una posición de nuestro array.*/
 while($row = mysqli_fetch_assoc($result)) {
    
    $datos[$i] = array($row['temps_date'],$row['temps_temp']);
    $i++;
 }


//print_r($datos);


/* una vez tengamos el array $datos listo, utilizamos la instrucción json_encode 
que será llamada desde nuestro segundo archivo */
echo json_encode($datos);


?>

