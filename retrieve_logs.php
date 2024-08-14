<?php

class Database 
{
    private $host = 'localhost';
    private $db_name = 'rfid_system';
    private $username = 'root';
    private $password = '';
    public $conn;

    public function getConnection() 
    {
        $this->conn = null;
        try 
        {
            $this->conn = new PDO("mysql:host=" . $this->host . ";dbname=" . $this->db_name, $this->username, $this->password);
            $this->conn->exec("set names utf8");
        } 
        catch(PDOException $exception) 
        {
            echo "Connection error: " . $exception->getMessage();
        }
        return $this->conn;
    }
}

class Log 
{
    private $conn;
    private $table_name = "user_logs";

    public function __construct($db) 
    {
        $this->conn = $db;
    }

    public function getLogs() 
    {
        $query = "SELECT * FROM " . $this->table_name . " ORDER BY timestamp DESC";
        $stmt = $this->conn->prepare($query);
        $stmt->execute();

        $logs = array();
        while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) 
        {
            $logs[] = $row;
        }
        return $logs;
    }
}

if ($_SERVER['REQUEST_METHOD'] === 'GET') 
{
    $database = new Database();
    $db = $database->getConnection();

    $log = new Log($db);
    $logs = $log->getLogs();

    echo json_encode($logs);
} 
else 
{
    echo json_encode(array("message" => "Invalid request method."));
}

?>
