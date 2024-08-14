<?php

class Database 
{
    private $host = "localhost";
    private $username = "root";
    private $password = "";
    private $dbname = "rfid_system";
    private $conn;

    public function connect() 
    {
        $this->conn = new mysqli($this->host, $this->username, $this->password, $this->dbname);
        if ($this->conn->connect_error) 
        {
            error_log("Connection failed: " . $this->conn->connect_error);
            die("Connection failed: " . $this->conn->connect_error);
        }
    }

    public function query($sql) 
    {
        $result = $this->conn->query($sql);
        if ($this->conn->error) 
        {
            error_log("Query error: " . $this->conn->error);
        }
        return $result;
    }
}

class RFIDLogger 
{
    private $db;

    public function __construct() 
    {
        $this->db = new Database();
        $this->db->connect();
    }

    public function log($rfid) 
    {
        $rfid = $this->db->conn->real_escape_string($rfid); // Sanitize input

        // Check if the RFID exists and retrieve the user info
        $sql = "SELECT * FROM users WHERE rfid='$rfid'";
        $result = $this->db->query($sql);

        if ($result && $result->num_rows > 0) 
        {
            // Check if user is already logged in
            $sql = "SELECT * FROM logs WHERE rfid='$rfid' AND logout_time IS NULL";
            $logResult = $this->db->query($sql);

            if ($logResult && $logResult->num_rows > 0) 
            {
                // Log out user
                $sql = "UPDATE logs SET logout_time=NOW() WHERE rfid='$rfid' AND logout_time IS NULL";
            } 
            else 
            {
                // Log in user
                $sql = "INSERT INTO logs (rfid, login_time) VALUES ('$rfid', NOW())";
            }
            if (!$this->db->query($sql)) 
            {
                echo "Error logging RFID data.";
                error_log("Error logging RFID data: " . $this->db->conn->error);
            }
        } 
        else 
        {
            echo "Access Denied: No registered account for this RFID.";
            error_log("Access Denied: No registered account for RFID: $rfid");
        }
    }
}

if (isset($_POST['rfid'])) 
{
    $logger = new RFIDLogger();
    $logger->log($_POST['rfid']);
} 
else 
{
    echo "No RFID data received.";
    error_log("No RFID data received.");
}

?>
