#include <mbgl/text/tagged_string.hpp>
#include <mbgl/util/i18n.hpp>

#include <boost/algorithm/string.hpp>

namespace mbgl {
    
void TaggedString::addSection(const std::u16string& sectionText, double scale, FontStackHash fontStack) {
    styledText.first += sectionText;
    sections.emplace_back(scale, fontStack);
    styledText.second.resize(styledText.first.size(), sections.size() - 1);
}

void TaggedString::trim() {
    auto whiteSpace = boost::algorithm::is_any_of(u" \t\n\v\f\r");
    std::size_t beginningWhitespace = 0;
    for (std::size_t i = 0;
         i < styledText.first.length() && whiteSpace(styledText.first.at(i));
         i++) {
        beginningWhitespace++;
    }
    std::size_t trailingWhitespace = styledText.first.length();
    for (int32_t i = styledText.first.length() - 1;
         i >= 0 && std::size_t(i) >= beginningWhitespace && whiteSpace(styledText.first.at(i));
         i--) {
        trailingWhitespace--;
    }
    styledText.first = styledText.first.substr(beginningWhitespace, trailingWhitespace - beginningWhitespace);
    styledText.second = std::vector<uint8_t>(styledText.second.begin() + beginningWhitespace, styledText.second.begin() + trailingWhitespace);
}

double TaggedString::getMaxScale() const {
    double maxScale = 0.0;
    for (std::size_t i = 0; i < styledText.first.length(); i++) {
        maxScale = std::max(maxScale, getSection(i).scale);
    }
    return maxScale;
}
    
void TaggedString::verticalizePunctuation() {
    // Relies on verticalization changing characters in place so that style indices don't need updating
    styledText.first = util::i18n::verticalizePunctuation(styledText.first);
}

} // namespace mbgl
