<?php

   // Check if the 'data' parameter is set in the URL
   if (isset($_GET['data']))
   {
      $data = $_GET['data'];
      echo "<hr>" . $data . "<hr>";



   $srvrname = "localhost";
   $database = "pm";
   $username = "pmdb_ro";
   $password = "readonly";

   // Create a connection
   $conn = mysqli_connect($srvrname, $username, $password, $database);

   // Check the connection
   if (!$conn) die("Connection failed: " . mysqli_connect_error());
   // echo "Connected to database\n";

   $sql = "SELECT filename FROM gm";
   global $conn;
   $result = mysqli_query($conn, $sql);

   if ($result->num_rows == 0) echo " none found \n";
   else
   {
      $row = $result->fetch_assoc();
      $filename = $row['filename'];  
      echo "filename: $filename\n";
   }





   }
   else
   {
       // Default content if no parameter is set
       echo "<hr>Default<hr>";
   }
?>

