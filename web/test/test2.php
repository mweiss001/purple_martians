<!DOCTYPE html>
<html>
<body>

<h1>Dynamic Content Example</h1>

<!-- Navigation Links -->
<a href="index.php?page=home">Home</a> | 
<a href="index.php?page=about">About</a> | 
<a href="index.php?page=contact">Contact</a>

<div id="content-area">
    <?php
    // Check if the 'page' parameter is set in the URL
    if (isset($_GET['page'])) {
        $page = $_GET['page'];

        // Use a whitelist to prevent directory traversal vulnerabilities
        $allowed_pages = ['home', 'about', 'contact'];

        if (in_array($page, $allowed_pages)) {
            // Include the corresponding PHP file
            include($page . '.php');
        } else {
            echo "<p>Content not found.</p>";
        }
    } else {
        // Default content if no parameter is set
        include('home.php');
    }
    ?>
</div>

</body>
</html>
