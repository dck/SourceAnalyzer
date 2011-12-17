#ifndef NODELOCATION_H
#define NODELOCATION_H

    #include "INodeLocation.h"

    class std::string;

    class NodeLocation : public INodeLocation {
        public:
            virtual size_t      getPos  () const;
            virtual size_t      getLine () const;
            virtual std::string getFile () const;

            virtual void setPos  ( const size_t pos  );
            virtual void setLine ( const size_t line );
            virtual void setFile ( const std::string& file );

        private:
            size_t       _pos;
            size_t       _line;
            std::string  _file;  
    };

#endif //NODELOCATION_H
