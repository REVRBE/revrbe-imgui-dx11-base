# imgui DX11 base with XenForo authentication

I really have no interest in updating my DX9 base, so I figured it would probably be better to release one for DX11 - hopefully with future updates to improve and/or add new stuff.

## Updates

I added a simple interface that is displayed on successful login. 

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

2. And I recommend using vcpkg for installing necessary static/dynamic libraries (json.h and curl.h) at the moment the project is using dynamic libraries, but its easy to change, just go and edit the settings under "vcpkg" in project settings.

```
// Dynamic libraries
/vcpkg install curl:x64-windows 
/vcpkg install jsoncpp:x64-windows

// Static libraries (use these if you want the program file to work anywhere on the PC)
/vcpkg install curl:x64-windows-static
/vcpkg install jsoncpp:x64-windows-static
```

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

## Ways to avoid database breach

Use a secure database password (it can be up to 128 characters - maybe even change it regularly)

Use a reliable/secure web-hosting

Add/make a script that notifies you with a mail to your personel/work mail-address when something fishy is going on in the database, so you can react quickly.

If you really want to go all-in, then look up Cloudflare WAF
