#ifndef __SKIPPER_HPP__
#define __SKIPPER_HPP__

#include <boost/spirit/include/qi.hpp>

namespace algc {
   namespace parser {
      namespace qi = boost::spirit::qi;
      namespace ascii = boost::spirit::ascii;

      ///////////////////////////////////////////////////////////////////////////////
      //  The skipper grammar
      ///////////////////////////////////////////////////////////////////////////////
      template <typename Iterator>
      struct skipper : qi::grammar<Iterator>
      {
         skipper() : skipper::base_type(start)
         {
            qi::char_type char_;
            ascii::space_type space;

            start =
                    space                               // tab/space/cr/lf
                |   "/*" >> *(char_ - "*/") >> "*/"     // C-style comments
                ;
         }

         qi::rule<Iterator> start;
      };
   }
}


#endif // __SKIPPER_HPP__
