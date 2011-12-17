#ifndef INODELOCATION_H
#define INODELOCATION_H

    class std::string;

    class INodeLocation {
        public:
            INodeLocation ();
            INodeLocation ( const size_t pos, 
                            const size_t line, 
                            const std::string& file );

            virtual size_t      getPos  () const = 0;
            virtual size_t      getLine () const = 0;
            virtual std::string getFile () const = 0;

            virtual void setPos  ( const size_t pos  ) = 0;
            virtual void setLine ( const size_t line ) = 0;
            virtual void setFile ( const std::string& file ) = 0;
    };

#endif //INODELOCATION_H
