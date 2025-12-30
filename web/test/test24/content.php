<?php
if (isset($_GET['page'])) {
    $page = $_GET['page'];
    $allowed_pages = ['home', 'about', 'contact'];

    if (in_array($page, $allowed_pages)) {
        // Return only the content snippet without a full HTML structure
        include($page . '.php');
    } else {
        echo "<p>Content not found.</p>";
    }
}
?>
