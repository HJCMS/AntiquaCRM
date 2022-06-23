#include "abebooksdocument.h"

AbeBooksDocument::AbeBooksDocument(const AbeBooksAccess &config,
                                   const QString &element) {
  apiLogin = config.user;
  apiKey = config.key;
  createProcessingInstruction("xml", "version=\"1.0\" encoding=\"ISO-8859-1\"");
  QDomElement actionNode = createElement(element);
  actionNode.setAttribute("version", "1.1");
  appendChild(actionNode);
}

const QDomElement AbeBooksDocument::createAction(const QString &tag) {
  QDomElement actionNode = createElement("action");
  if (tag.isEmpty())
    actionNode.setAttribute("name", "getAllNewOrders");
  else
    actionNode.setAttribute("name", tag);

  QDomElement username = createElement("username");
  username.appendChild(createTextNode(apiLogin));
  actionNode.appendChild(username);

  QDomElement password = createElement("password");
  password.appendChild(createTextNode(apiKey));
  actionNode.appendChild(password);

  documentElement().appendChild(actionNode);
  return actionNode;
}
