<?php
// Function to sanitize and get the 'page' parameter from the URL
function get_page_content($page) {
    switch ($page) {
        case 'about':
            return 'include/about.php';
        case 'contact':
            return 'include/contact.php';
        case 'home':
        default:
            return 'include/home.php';
    }
}

// Get the page parameter, default to 'home' if not set
$page = isset($_GET['page']) ? $_GET['page'] : 'home';
$content_file = get_page_content($page);
?>

<!DOCTYPE html>
<html>
<head>
    <title>Dynamic Content</title>
</head>
<body>

<nav>
    <!-- Links with query strings -->
    <a href="index.php?page=home">Home</a> |
    <a href="index.php?page=about">About</a> |
    <a href="index.php?page=contact">Contact</a>
</nav>

<div id="content-container">
    <?php
    // Include the content file
    if (file_exists($content_file)) {
        include($content_file);
    } else {
        echo "<p>Content not found.</p>";
    }
    ?>
</div>

</body>
</html>
