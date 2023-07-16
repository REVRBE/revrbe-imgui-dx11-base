# imgui DX11 base with XenForo authentication

I really have no interest in updating my DX9 base, so I figured it would probably be better to release one for DX11 - hopefully with future updates to improve and/or add new stuff.

## Features

✅ XenForo authentication (returning highest rank with the default login.php)  
✅ DragonByte support (returning all licenses; license id, product id and expiry date with the DragonByte login.php file)  
✅ Color schemes readded to DX11 base; + remembers last color  
✅ TLS/SSL encryption  
✅ Medium protection against SQL injections  
✅ DX11  
❌ Using returned data to display it  

## Installing

1. Make sure to have DirectX SDK installed

2. And I recommend using vcpkg for installing necessary static/dynamic libraries (json.h and curl.h)

3. Also make sure to add these lines to ".htaccess" file (necessary to work):

```
SetEnv DB_SERVERNAME your_servername // change it to your database settings
SetEnv DB_USERNAME your_username
SetEnv DB_PASSWORD your_password
SetEnv DB_NAME your_db_name
```
4. Add this to your database to make the max login attempts work (against brute-forcing)
```
// and also add this for extra sql injection protection
 
 USE your_database_name; // insert your database name

CREATE TABLE xf_login_attempts (
    id INT AUTO_INCREMENT PRIMARY KEY,
    username VARCHAR(255) NOT NULL,
    login_attempts INT NOT NULL DEFAULT 0,
    last_login_attempt TIMESTAMP NOT NULL DEFAULT CURRENT_TIMESTAMP,
    UNIQUE (username)
);
```

I'll finish the README later
