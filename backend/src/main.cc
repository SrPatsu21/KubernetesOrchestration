#include <drogon/drogon.h>
#include <vector>
#include <string>

const char* host = std::getenv("DB_HOST");
const char* port = std::getenv("DB_PORT");
const char* db   = std::getenv("DB_NAME");
const char* user = std::getenv("DB_USER");
const char* pass = std::getenv("DB_PASSWORD");

int main() {
    std::string conn =
        "host=" + std::string(host) +
        " port=" + std::string(port) +
        " dbname=" + std::string(db) +
        " user=" + std::string(user) +
        " password=" + std::string(pass);

    auto dbClient = drogon::orm::DbClient::newPgClient(conn, 4);

    dbClient->execSqlSync(R"(
        CREATE TABLE IF NOT EXISTS items (
            id SERIAL PRIMARY KEY,
            name VARCHAR(255) NOT NULL
        )
    )");

    // GET /items
    drogon::app().registerHandler(
        "/items",
        [dbClient](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)> &&callback)
        {
            dbClient->execSqlAsync(
                "SELECT id, name FROM items ORDER BY id",

                [callback](const drogon::orm::Result &result)
                {
                    Json::Value response(Json::arrayValue);

                    for (const auto &row : result)
                    {
                        Json::Value obj;
                        obj["id"] = row["id"].as<int>();
                        obj["name"] = row["name"].as<std::string>();

                        response.append(obj);
                    }

                    auto resp =
                        drogon::HttpResponse::newHttpJsonResponse(response);

                    callback(resp);
                },

                [callback](const drogon::orm::DrogonDbException &e)
                {
                    auto resp = drogon::HttpResponse::newHttpResponse();

                    resp->setStatusCode(
                        drogon::k500InternalServerError);

                    resp->setBody(e.base().what());

                    callback(resp);
                });
        },
        {drogon::Get}
    );

    // POST /items
    drogon::app().registerHandler(
        "/items",
        [dbClient](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)> &&callback)
        {
            auto json = req->getJsonObject();

            if (!json || !json->isMember("name"))
            {
                auto resp = drogon::HttpResponse::newHttpResponse();

                resp->setStatusCode(drogon::k400BadRequest);
                resp->setBody("Campo 'name' obrigatório");

                callback(resp);
                return;
            }

            std::string name = (*json)["name"].asString();

            dbClient->execSqlAsync(
                "INSERT INTO items(name) VALUES($1) RETURNING id",
                [callback](const drogon::orm::Result &result)
                {
                    Json::Value response;

                    response["message"] = "Item criado";
                    response["id"] = result[0]["id"].as<int>();

                    auto resp =
                        drogon::HttpResponse::newHttpJsonResponse(response);

                    callback(resp);
                },
                [callback](const drogon::orm::DrogonDbException &e)
                {
                    auto resp = drogon::HttpResponse::newHttpResponse();

                    resp->setStatusCode(
                        drogon::k500InternalServerError);

                    resp->setBody(e.base().what());

                    callback(resp);
                },
                name);
        },
        {drogon::Post}
    );

    // DELETE /items/{id}
    drogon::app().registerHandler(
        "/items/{id}",
        [dbClient](
            const drogon::HttpRequestPtr& req,
            std::function<void(const drogon::HttpResponsePtr&)> &&callback,
            int id)
        {
            dbClient->execSqlAsync(
                "DELETE FROM items WHERE id = $1",

                [callback](const drogon::orm::Result &result)
                {
                    Json::Value response;

                    response["message"] = "Item removido";

                    auto resp =
                        drogon::HttpResponse::newHttpJsonResponse(response);

                    callback(resp);
                },

                [callback](const drogon::orm::DrogonDbException &e)
                {
                    auto resp = drogon::HttpResponse::newHttpResponse();

                    resp->setStatusCode(
                        drogon::k500InternalServerError);

                    resp->setBody(e.base().what());

                    callback(resp);
                },

                id);
        },
        {drogon::Delete}
    );

    drogon::app().registerPostHandlingAdvice([](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp)
    {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET,POST,DELETE,OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
    });

    drogon::app().registerSyncAdvice(
        [](const drogon::HttpRequestPtr &req)->drogon::HttpResponsePtr
        {
            if (req->method() == drogon::Options)
            {
                auto resp = drogon::HttpResponse::newHttpResponse();

                resp->addHeader("Access-Control-Allow-Origin", "*");
                resp->addHeader("Access-Control-Allow-Methods", "GET,POST,DELETE,OPTIONS");
                resp->addHeader("Access-Control-Allow-Headers", "Content-Type");

                resp->setStatusCode(drogon::k200OK);

                return resp;
            }

            return {};
        }
    );

    drogon::app().addListener("0.0.0.0", 8080);

    LOG_INFO << "API rodando na porta 8080";

    drogon::app().run();

    return 0;
}