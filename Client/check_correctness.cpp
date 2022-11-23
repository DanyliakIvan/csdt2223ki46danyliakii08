#include "check_correctness.h"

QString checkmessage(QString message)
{
    if(message.size() > 30)
    {
        return "Message must be less than 31 characters!";
    }
    else
    {
        QRegularExpression re("[a-zA-Z0-9]+");
        QRegularExpressionMatch match = re.match(message);

        if(match.hasMatch())
        {
             return message;
        }
        else
        {
            return "Message must contain only latin characters!";
        }

    }
}
