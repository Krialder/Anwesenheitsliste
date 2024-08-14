CREATE DATABASE IF NOT EXISTS rfid_system;

USE rfid_system;

-- Users Table
CREATE TABLE IF NOT EXISTS users 
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    rfid VARCHAR(255) NOT NULL UNIQUE,
    name VARCHAR(255) NOT NULL,
    classification_id INT NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (classification_id) REFERENCES classifications(id)
);

-- Classifications Table
CREATE TABLE IF NOT EXISTS classifications 
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    name VARCHAR(255) NOT NULL UNIQUE
);

-- Logs Table
CREATE TABLE IF NOT EXISTS logs 
(
    id INT AUTO_INCREMENT PRIMARY KEY,
    rfid VARCHAR(255) NOT NULL UNIQUE,
    login_time DATETIME DEFAULT CURRENT_TIMESTAMP,
    logout_time DATETIME DEFAULT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    updated_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,
    FOREIGN KEY (rfid) REFERENCES users(rfid)
);

-- Indexes for performance improvement
CREATE INDEX idx_users_rfid ON users(rfid);
CREATE INDEX idx_logs_rfid ON logs(rfid);

-- Insert sample data into classifications
INSERT INTO classifications (name) VALUES
('BVB'),
('Auszubildende'),
('Ausbilder'),
('Admin');

-- Insert sample data into users
INSERT INTO users (rfid, name, classification_id) VALUES
('1234abcd', 'John Doe', (SELECT id FROM classifications WHERE name = 'BVB')),
('5678efgh', 'Jane Doe', (SELECT id FROM classifications WHERE name = 'Auszubildende')),
('9101ijkl', 'Max Mustermann', (SELECT id FROM classifications WHERE name = 'Ausbilder')),
('1213mnop', 'Erika Mustermann', (SELECT id FROM classifications WHERE name = 'Admin'));
