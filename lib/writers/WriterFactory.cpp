//
//  Created by Thomas Rambrant, 2026
//  This project is licensed under the MIT License - see the LICENSE file for details.
//
#include "WriterFactory.hpp"

#include "core/Logger.hpp"
#include "WriterList.hpp"   // ← delete this line when migrating to P2996

namespace com::rambrant::sudoku
{
    template<WriterPlugin T>
    auto WriterFactory::makeCreator() -> WriterFactory::CreatorFn
    {
        return []( std::ostream& os, const Logger& logger) -> std::unique_ptr<IWriter>
        {
            return std::make_unique<T>( os, logger);
        };
    }

    WriterFactory::WriterFactory()
    {
        populate(
            std::type_identity<WriterList>{},
            []<typename T>{ return T::formatName; }
        );
    }

    auto WriterFactory::create( std::string_view format,
                                std::ostream&    os,
                                const Logger&    logger) const
        -> std::expected<std::unique_ptr<IWriter>, std::string>
    {
        const auto* creator = findCreator( format);
        if( !creator)
            return std::unexpected{ "Unknown writer format: " + std::string{ format}};
        return (*creator)( os, logger);
    }

    auto WriterFactory::formats() const -> std::vector<std::string>
    {
        return registeredKeys();
    }
}