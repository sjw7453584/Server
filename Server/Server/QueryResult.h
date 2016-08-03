/* QueryResult.h

 */

#if !defined(QUERYRESULT_H)
#define QUERYRESULT_H
#include "Types.h"
#include "Field.h"
#include <unordered_map>
#include <vector>
class QueryResult
{
    public:
        QueryResult() {}

        //! Frees resources used by QueryResult.
        virtual ~QueryResult() {}
		void AddField(uint32 rowIndex, Field& field)
		{
			rows[rowIndex].push_back(field);
		}
		std::unordered_map<uint32, std::vector<Field>> AllRows()
		{
			return rows;
		}
    protected:
		std::unordered_map<uint32, std::vector<Field>> rows;
};
#endif
