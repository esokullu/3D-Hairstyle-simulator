q:
CREATE TABLE `migrations` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  `filename` text NOT NULL
);

q:
CREATE TABLE `categories` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  `name` TEXT NOT NULL,
  `description` TEXT NOT NULL
);

q:
CREATE TABLE `category_relations` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  `parent_id` INTEGER NOT NULL,
  `child_id` INTEGER NOT NULL
);

q:
CREATE TABLE `hairstyles` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  `name` TEXT NOT NULL,
  `description` TEXT NOT NULL,
  `texture_filename` TEXT NOT NULL,
  `model_filename` TEXT NOT NULL
);

q:
CREATE TABLE `hairstyle_categories` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  `hairstyle_id` INTEGER NOT NULL,
  `category_id` INTEGER NOT NULL
);

q:
CREATE TABLE `hairstyle_attributes` (
  `id` INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL,
  `hairstyle_id` INTEGER NOT NULL,
  `key` TEXT NOT NULL,
  `value` TEXT NOT NULL
);

q: INSERT INTO `categories` (`name`, `description`) VALUES ('Причёски', '');
q: INSERT INTO `categories` (`name`, `description`) VALUES ('Мужские', '');
q: INSERT INTO `categories` (`name`, `description`) VALUES ('Женские', '');
q: INSERT INTO `categories` (`name`, `description`) VALUES ('Короткие', '');
q: INSERT INTO `categories` (`name`, `description`) VALUES ('Средние', '');
q: INSERT INTO `categories` (`name`, `description`) VALUES ('Длинные', '');

q:
INSERT INTO `category_relations` (`parent_id`, `child_id`)
  WITH
    c1 AS (SELECT `id` FROM `categories` WHERE `name` = 'Причёски'),
    c2 AS (SELECT `id` FROM `categories` WHERE `name` = 'Мужские')
  SELECT c1.id, c2.id FROM c1, c2;

q:
INSERT INTO `category_relations` (`parent_id`, `child_id`)
  WITH
    c1 AS (SELECT `id` FROM `categories` WHERE `name` = 'Мужские'),
    c2 AS (SELECT `id` FROM `categories` WHERE `name` = 'Короткие')
  SELECT c1.id, c2.id FROM c1, c2;

q:
INSERT INTO `category_relations` (`parent_id`, `child_id`)
  WITH
    c1 AS (SELECT `id` FROM `categories` WHERE `name` = 'Мужские'),
    c2 AS (SELECT `id` FROM `categories` WHERE `name` = 'Средние')
  SELECT c1.id, c2.id FROM c1, c2;

q:
INSERT INTO `category_relations` (`parent_id`, `child_id`)
  WITH
    c1 AS (SELECT `id` FROM `categories` WHERE `name` = 'Мужские'),
    c2 AS (SELECT `id` FROM `categories` WHERE `name` = 'Длинные')
  SELECT c1.id, c2.id FROM c1, c2;

q:
INSERT INTO `category_relations` (`parent_id`, `child_id`)
  WITH
    c1 AS (SELECT `id` FROM `categories` WHERE `name` = 'Причёски'),
    c2 AS (SELECT `id` FROM `categories` WHERE `name` = 'Женские')
  SELECT c1.id, c2.id FROM c1, c2;

q:
INSERT INTO `category_relations` (`parent_id`, `child_id`)
  WITH
    c1 AS (SELECT `id` FROM `categories` WHERE `name` = 'Женские'),
    c2 AS (SELECT `id` FROM `categories` WHERE `name` = 'Короткие')
  SELECT c1.id, c2.id FROM c1, c2;

q:
INSERT INTO `category_relations` (`parent_id`, `child_id`)
  WITH
    c1 AS (SELECT `id` FROM `categories` WHERE `name` = 'Женские'),
    c2 AS (SELECT `id` FROM `categories` WHERE `name` = 'Средние')
  SELECT c1.id, c2.id FROM c1, c2;

q:
INSERT INTO `category_relations` (`parent_id`, `child_id`)
  WITH
    c1 AS (SELECT `id` FROM `categories` WHERE `name` = 'Женские'),
    c2 AS (SELECT `id` FROM `categories` WHERE `name` = 'Длинные')
  SELECT c1.id, c2.id FROM c1, c2;
