CREATE TABLE IF NOT EXISTS Users (
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
    Username TEXT NOT NULL, 
    Password TEXT
);

CREATE TABLE IF NOT EXISTS ContactList (
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
    IdUser INTEGER NOT NULL,
    IdContact INTEGER NOT NULL,
    DateCR Date
);

CREATE TABLE IF NOT EXISTS PrivateCallsLog (
    Id INTEGER PRIMARY KEY AUTOINCREMENT,
    IdUser INTEGER NOT NULL,
    IdContact INTEGER NOT NULL,
    DateCR Date,
    DateHangUp Date
);