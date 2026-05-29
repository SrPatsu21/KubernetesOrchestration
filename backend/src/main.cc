#include <drogon/drogon.h>
#include <vector>
#include <string>

struct Item {
    int id;
    std::string name;
};

std::vector<Item> items = {
    {1, "Notebook"},
    {2, "Mouse"}
};

int main() {

    // GET /items
    drogon::app().registerHandler(
        "/items",
        [](const drogon::HttpRequestPtr& req,
           std::function<void (const drogon::HttpResponsePtr &)> &&callback)
        {
            Json::Value response;

            for (const auto& item : items) {
                Json::Value obj;
                obj["id"] = item.id;
                obj["name"] = item.name;
                response.append(obj);
            }

            auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
            callback(resp);
        },
        {drogon::Get}
    );

    // POST /items
    drogon::app().registerHandler(
        "/items",
        [](const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback)
        {
            auto json = req->getJsonObject();

            if (!json || !json->isMember("name")) {
                auto resp = drogon::HttpResponse::newHttpResponse();
                resp->setStatusCode(drogon::k400BadRequest);
                resp->setBody("Campo 'name' obrigatório");
                callback(resp);
                return;
            }

            Item item;
            item.id = items.size() + 1;
            item.name = (*json)["name"].asString();

            items.push_back(item);

            Json::Value response;
            response["message"] = "Item criado";
            response["id"] = item.id;

            auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
            callback(resp);
        },
        {drogon::Post}
    );

    // DELETE /items/{id}
    drogon::app().registerHandler(
        "/items/{id}",
        [](const drogon::HttpRequestPtr& req, std::function<void (const drogon::HttpResponsePtr &)> &&callback, int id)
        {
            for (auto it = items.begin(); it != items.end(); ++it) {
                if (it->id == id) {
                    items.erase(it);

                    Json::Value response;
                    response["message"] = "Item removido";

                    auto resp = drogon::HttpResponse::newHttpJsonResponse(response);
                    callback(resp);
                    return;
                }
            }

            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setStatusCode(drogon::k404NotFound);
            resp->setBody("Item não encontrado");
            callback(resp);
        },
        {drogon::Delete}
    );

    drogon::app().registerPostHandlingAdvice([](const drogon::HttpRequestPtr &req, const drogon::HttpResponsePtr &resp)
    {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET,POST,DELETE,OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "Content-Type");
    });

    drogon::app().addListener("0.0.0.0", 8080);

    LOG_INFO << "API rodando na porta 8080";

    drogon::app().run();

    return 0;
}