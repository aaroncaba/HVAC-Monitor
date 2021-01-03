-- phpMyAdmin SQL Dump
-- version 5.0.2
-- https://www.phpmyadmin.net/
--
-- Host: 127.0.0.1:3306
-- Generation Time: Jan 03, 2021 at 01:59 AM
-- Server version: 10.5.8-MariaDB
-- PHP Version: 7.4.13

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Database: `hvac`
--

-- --------------------------------------------------------

--
-- Table structure for table `devices`
--

DROP TABLE IF EXISTS `devices`;
CREATE TABLE IF NOT EXISTS `devices` (
  `id` smallint(5) UNSIGNED NOT NULL AUTO_INCREMENT,
  `name` varchar(50) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=5 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `devices`
--

INSERT INTO `devices` (`id`, `name`) VALUES
(1, 'Main Floor'),
(2, 'Basement'),
(3, 'Upstairs'),
(4, 'HVAC Controller');

-- --------------------------------------------------------

--
-- Table structure for table `device_commands`
--

DROP TABLE IF EXISTS `device_commands`;
CREATE TABLE IF NOT EXISTS `device_commands` (
  `id` smallint(5) UNSIGNED NOT NULL AUTO_INCREMENT,
  `command` varchar(20) NOT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=8 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `device_commands`
--

INSERT INTO `device_commands` (`id`, `command`) VALUES
(1, 'Heat 1'),
(2, 'Heat 2'),
(3, 'Fan'),
(4, 'Setpoint'),
(5, 'Cool 1'),
(6, 'Cool 2'),
(7, 'Emergency Heat');

-- --------------------------------------------------------

--
-- Table structure for table `signals`
--

DROP TABLE IF EXISTS `signals`;
CREATE TABLE IF NOT EXISTS `signals` (
  `id` smallint(5) UNSIGNED NOT NULL AUTO_INCREMENT,
  `name` varchar(20) NOT NULL,
  `device_id` smallint(5) UNSIGNED NOT NULL,
  `command_id` smallint(5) UNSIGNED NOT NULL,
  PRIMARY KEY (`id`),
  KEY `device_id` (`device_id`),
  KEY `command_id` (`command_id`)
) ENGINE=InnoDB AUTO_INCREMENT=14 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `signals`
--

INSERT INTO `signals` (`id`, `name`, `device_id`, `command_id`) VALUES
(1, 'Main Floor Heat', 1, 1),
(2, 'Main Floor Cool', 1, 5),
(3, 'Main Floor Fan', 1, 3),
(4, 'Basement Heat', 2, 1),
(5, 'Basement Cool', 2, 5),
(6, 'Basement Fan', 2, 3),
(7, 'Upstairs Heat', 3, 1),
(8, 'Upstairs Cool', 3, 5),
(9, 'Upstairs Fan', 3, 3),
(10, 'HVAC Heat 1', 4, 1),
(11, 'HVAC Heat 2', 4, 2),
(12, 'HVAC Emergency Heat', 4, 7),
(13, 'HVAC Controller Cool', 4, 5);

-- --------------------------------------------------------

--
-- Table structure for table `states`
--

DROP TABLE IF EXISTS `states`;
CREATE TABLE IF NOT EXISTS `states` (
  `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT,
  `signal_id` smallint(5) UNSIGNED NOT NULL,
  `state` enum('ON','OFF') NOT NULL,
  `time` timestamp NOT NULL DEFAULT current_timestamp(),
  PRIMARY KEY (`id`),
  KEY `signal_id_index` (`signal_id`)
) ENGINE=InnoDB AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;

--
-- Dumping data for table `states`
--

INSERT INTO `states` (`id`, `signal_id`, `state`, `time`) VALUES
(1, 6, 'ON', '2021-01-02 00:45:01'),
(2, 1, 'ON', '2021-01-02 00:45:18'),
(3, 1, 'OFF', '2021-01-02 00:45:26'),
(4, 9, 'ON', '2021-01-02 00:47:44'),
(5, 9, 'OFF', '2021-01-02 00:47:49'),
(6, 1, 'ON', '2021-01-02 21:01:31'),
(7, 2, 'ON', '2021-01-02 21:01:46'),
(8, 3, 'ON', '2021-01-02 21:01:52'),
(9, 4, 'OFF', '2021-01-03 01:23:47'),
(10, 4, 'ON', '2021-01-03 01:23:52'),
(11, 4, 'OFF', '2021-01-03 01:23:56');

--
-- Constraints for dumped tables
--

--
-- Constraints for table `signals`
--
ALTER TABLE `signals`
  ADD CONSTRAINT `fk_signals_commands` FOREIGN KEY (`command_id`) REFERENCES `device_commands` (`id`) ON UPDATE CASCADE,
  ADD CONSTRAINT `fk_signals_devices` FOREIGN KEY (`device_id`) REFERENCES `devices` (`id`) ON UPDATE CASCADE;

--
-- Constraints for table `states`
--
ALTER TABLE `states`
  ADD CONSTRAINT `fk_states_signals` FOREIGN KEY (`signal_id`) REFERENCES `signals` (`id`) ON DELETE CASCADE ON UPDATE CASCADE;
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
