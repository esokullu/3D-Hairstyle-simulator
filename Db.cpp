#include "Db.h"
#include <QtSql>
#include <QDebug>
#include <QDirIterator>
#include <QSqlDatabase>
#include <QString>
#include <QFileInfo>
#include <QSqlQuery>
#include <QTextCodec>
#include <QList>
#include "models/Category.h"
#include "models/Hairstyle.h"

Db::Db(const QString databaseName)
{
    if (!db.isOpen()) {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName(databaseName);
        if (!db.open()) {
            qDebug() << "Unable to load db: " + databaseName;
            return;
        }
    }

    bool installed = false;
    auto tables = db.tables();
    for (int i = 0; i < tables.size() && !installed; ++i) {
        installed = (tables[i] == "migrations");
    }
    if (!installed) {
        applySqlFile(":/migrations/setup.sql");
    }

    QDirIterator dir(":/migrations");
    QStringList migrations;
    while (dir.hasNext()) { // TODO *.sql
        migrations.append(dir.next());
    }
    migrations.sort();
    for (int i = 0; i < migrations.size(); ++i) {
        if (migrations[i].indexOf("migration"))
        applySqlFile(migrations[i]);
    }
}

void Db::applySqlFile(const QString filename) {
    QFile migrationFile(filename);
    if (!migrationFile.open(QIODevice::ReadOnly)) {
        qDebug() << "Unable to load migration: " + filename;
    }
    QTextStream migrationStream(&migrationFile);
    QString queriesString;

    int i = 0;
    QString separator;
    migrationStream.setCodec("UTF-8");
    while (!migrationStream.atEnd()) {
        if (i > 0)
            queriesString += migrationStream.readLine();
        else
            separator = migrationStream.readLine();
        ++i;
    }
    migrationFile.close();

    // TODO try-catch?
    db.exec("BEGIN");
    bool ok = true;
    foreach (QString query, queriesString.split(separator)) {
        QSqlQuery q;
        ok = ok && q.prepare(query) && q.exec();
    }
    QSqlQuery q("INSERT INTO `migrations`(`filename`) VALUES(?);");
    q.addBindValue((new QFileInfo(filename))->baseName());
    ok = ok && q.exec();

    if (ok) {
        qDebug() << "Success: Migration " + filename + " migrated";
        db.exec("COMMIT");
    } else {
        qDebug() << db.lastError().text();
        qDebug() << "Fail: Migration " + filename + " migrated";
        db.exec("ROLLBACK");
    }
}

Category* Db::getRootCategory() {
    auto result = db.exec("SELECT id, name, descripion FROM categories WHERE id IN (SELECT parent_id FROM category_relations) AND WHERE NOT IN (SELECT child_id FROM category_relations)");
    while (result.next()) {
        auto category = new Category();
        category->id = result.value(0).toInt();
        category->name = result.value(1).toString();
        category->description = result.value(2).toString();
        return category;
    }
    return nullptr;
}

std::vector<Category*> Db::getSubcategoriesByCategoryId(int categoryId) {
    QSqlQuery q("SELECT id, name, description FROM categories WHERE id IN (SELECT parent_id FROM category_relations WHERE child_id = ?)");
    q.addBindValue(categoryId);
    q.exec();
    auto result = q.result();
    std::vector<Category*> categories;
    while (q.next()) {
        auto category = new Category();
        category->id = q.value(0).toInt();
        category->name = q.value(1).toString();
        category->description = q.value(2).toString();
        categories.push_back(category);
    }
    return categories;
}

std::vector<Hairstyle*> Db::getHairstylesByCategoryIds(QList<int> categoryIds) {
    QSqlQuery q("SELECT id, name, description, texture_filename, model_filename FROM hairstyles WHERE id IN (SELECT hairstyle_id FROM hairstyle_categories WHERE category_id IN ALL ?)");
    QList<QVariant> listValue;
    for (int i = 0; i < categoryIds.size(); ++i) {
        listValue.append(categoryIds[i]);
    }
    q.addBindValue(listValue);
    q.exec();
    std::vector<Hairstyle*> hairstyles;
    while (q.next()) {
        Hairstyle* hairstyle = new Hairstyle();
        hairstyle->id = q.value(0).toInt();
        hairstyle->name = q.value(1).toString();
        hairstyle->description = q.value(2).toString();
        hairstyle->textureFilename = q.value(3).toString();
        hairstyle->modelFilename = q.value(4).toString();
        hairstyles.push_back(hairstyle);
    }
    return hairstyles;
}
