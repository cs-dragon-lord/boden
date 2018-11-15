#ifndef BDN_TEST_TestTextViewCore_H_
#define BDN_TEST_TestTextViewCore_H_

#include <bdn/TextView.h>
#include <bdn/test/TestViewCore.h>
#include <bdn/TextView.h>

namespace bdn
{
    namespace test
    {

        /** Helper for tests that verify ITextViewCore implementations.*/
        class TestTextViewCore : public TestViewCore<TextView>
        {

          protected:
            P<View> createView() override
            {
                P<TextView> pTextView = newObj<TextView>();

                pTextView->setText("hello world");

                return pTextView;
            }

            void setView(View *pView) override
            {
                TestViewCore::setView(pView);

                _pTextView = cast<TextView>(pView);
            }

            /** Returns true if the text view implementation will never return a
               preferred width that exceeds the available width, even if the
               text cannot be wrapped to make it smaller than the available
               width.

                Note that the preferred behaviour is for the text view to report
               a bigger preferred size if the text cannot be wrapped to fit the
               available width. I.e. the preferred behaviour is the one that
               corresponds to clipsPreferredWidthToAvailableWidth returning
               false.

                The default implementation returns false.
                */
            virtual bool clipsPreferredWidthToAvailableWidth() const { return false; }

            /** Returns true if the text view implementation will wrap at
               character boundaries if a single word does not fit in the
               available space. The preferred behaviour is to NOT wrap at
               character boundaries and instead clip the word.

                The default implementation returns false.
                */
            virtual bool wrapsAtCharacterBoundariesIfWordDoesNotFit() const { return false; }

            /** Returns true if the text view implementation uses all the
               available width when the text is wrapped (i.e. if the
               availableWidth is smaller than the text view's unconstrained
               width).

                The preferred behaviour would be for the text view to use the
               width that is actually needed for the wrapped	text (which
               usually ends up being slightly smaller than availableWidth). But
               some implementations cannot do that and will always return
               exactly availableWidth if the availableWidth is smaller than the
               unconstrained width.

                The default implementation returns false.
                */
            virtual bool usesAllAvailableWidthWhenWrapped() const { return false; }

            void runInitTests() override
            {
                TestViewCore::runInitTests();

                SECTION("text")
                {
                    _pTextView->setText("helloworld");
                    initCore();
                    verifyCoreText();
                }
            }

            void testPreferredSize(int availableHeight)
            {
                // note that the available height should have NO influence on
                // the result, since the text cannot be made smaller in height
                // (only in width by line-breaking)
                P<TestTextViewCore> pThis = this;

                SECTION("wider text causes wider preferred size")
                {
                    _pTextView->setText("");

                    CONTINUE_SECTION_WHEN_IDLE(pThis, availableHeight)
                    {
                        Size prefSizeBefore =
                            pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                        pThis->_pTextView->setText("helloworld");

                        CONTINUE_SECTION_WHEN_IDLE(pThis, prefSizeBefore, availableHeight)
                        {
                            Size prefSize =
                                pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                            // width must increase with a bigger text
                            REQUIRE(prefSize.width > prefSizeBefore.width);

                            // note that the height might or might not increase.
                            // But it cannot be smaller.
                            REQUIRE(prefSize.height >= prefSizeBefore.height);

                            // when we go back to the same text as before then
                            // the preferred size should also be the same again
                            pThis->_pTextView->setText("");

                            CONTINUE_SECTION_WHEN_IDLE(pThis, prefSizeBefore, availableHeight)
                            {
                                REQUIRE(pThis->_pTextView->calcPreferredSize(
                                            Size(Size::componentNone(), availableHeight)) == prefSizeBefore);
                            };
                        };
                    };
                }

                SECTION("linebreaks cause multiline")
                {
                    _pTextView->setText("");

                    CONTINUE_SECTION_WHEN_IDLE(pThis, availableHeight)
                    {
                        Size emptyTextPreferredSize =
                            pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                        pThis->_pTextView->setText("hello");

                        CONTINUE_SECTION_WHEN_IDLE(pThis, emptyTextPreferredSize, availableHeight)
                        {
                            Size prefSizeBefore =
                                pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                            // we put a little less text in the first line. On
                            // some systems the text view will allocate space
                            // for the linebreak at the end of the first line,
                            // which would cause our width test below to fail.
                            // So to avoid that we make the first line
                            // considerably shorter, thus causing the whole
                            // width of the text view to be measured according
                            // to the second line (which should have exactly the
                            // same width as the single line above).
                            pThis->_pTextView->setText("he\nhello");

                            CONTINUE_SECTION_WHEN_IDLE(pThis, prefSizeBefore, availableHeight, emptyTextPreferredSize)
                            {
                                Size prefSize =
                                    pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                                // should have same width as before (since both
                                // lines have the same texts).
                                REQUIRE(prefSize.width == prefSizeBefore.width);

                                // should have roughly twice the height as
                                // before. Note that the exact height can vary
                                // because the line spacing can be different.

                                REQUIRE(prefSize.height >= prefSizeBefore.height * 1.8);
                                REQUIRE(prefSize.height < prefSizeBefore.height * 3);

                                // when we go back to empty text then we should
                                // get the original size
                                pThis->_pTextView->setText("");

                                CONTINUE_SECTION_WHEN_IDLE(pThis, availableHeight, emptyTextPreferredSize)
                                {
                                    REQUIRE(pThis->_pTextView->calcPreferredSize(Size(
                                                Size::componentNone(), availableHeight)) == emptyTextPreferredSize);
                                };
                            };
                        };
                    };
                }

                SECTION("CRLF same as LF")
                {
                    _pTextView->setText("hello world\nbla");

                    CONTINUE_SECTION_WHEN_IDLE(pThis, availableHeight)
                    {
                        Size sizeLF =
                            pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                        pThis->_pTextView->setText("hello world\r\nbla");

                        CONTINUE_SECTION_WHEN_IDLE(pThis, sizeLF, availableHeight)
                        {
                            Size sizeCRLF =
                                pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                            REQUIRE(sizeLF == sizeCRLF);
                        };
                    };
                }

                SECTION("availableWidth has no effect if bigger than "
                        "unconstrained width")
                {
                    _pTextView->setText("hello world");

                    CONTINUE_SECTION_WHEN_IDLE(pThis, availableHeight)
                    {
                        Size unconstrainedSize =
                            pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                        REQUIRE(pThis->_pTextView->calcPreferredSize(
                                    Size(unconstrainedSize.width + 10, availableHeight)) == unconstrainedSize);
                    };
                }

                SECTION("availableWidth has no effect if equal to "
                        "unconstrained width")
                {
                    _pTextView->setText("hello world");

                    CONTINUE_SECTION_WHEN_IDLE(pThis, availableHeight)
                    {
                        Size unconstrainedSize =
                            pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                        REQUIRE(pThis->_pTextView->calcPreferredSize(Size(unconstrainedSize.width, availableHeight)) ==
                                unconstrainedSize);
                    };
                }

                SECTION("smaller availableWidth causes word wrap")
                {
                    _pTextView->setText("hellohello worldworld\nblabb");

                    CONTINUE_SECTION_WHEN_IDLE(pThis, availableHeight)
                    {
                        Size wrappedAtSecondPositionSize =
                            pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                        pThis->_pTextView->setText("hellohello\nworldworld blabb");

                        CONTINUE_SECTION_WHEN_IDLE(pThis, wrappedAtSecondPositionSize, availableHeight)
                        {
                            Size wrappedAtFirstPositionSize =
                                pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                            pThis->_pTextView->setText("hellohello worldworld blabb");

                            CONTINUE_SECTION_WHEN_IDLE(pThis, wrappedAtSecondPositionSize, wrappedAtFirstPositionSize,
                                                       availableHeight)
                            {
                                Size fullSize =
                                    pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                                REQUIRE(fullSize.width > wrappedAtSecondPositionSize.width);
                                REQUIRE(fullSize.height < wrappedAtSecondPositionSize.height);

                                // note that there might be some rounding errors
                                // with the width. So we accept 1 pixel
                                // difference
                                REQUIRE_ALMOST_EQUAL(pThis->_pTextView->calcPreferredSize(
                                                         Size(wrappedAtSecondPositionSize.width + 1, availableHeight)),
                                                     wrappedAtSecondPositionSize, Size(1, 0));

                                REQUIRE_ALMOST_EQUAL(pThis->_pTextView->calcPreferredSize(
                                                         Size(wrappedAtSecondPositionSize.width, availableHeight)),
                                                     wrappedAtSecondPositionSize, Size(1, 0));

                                if (pThis->usesAllAvailableWidthWhenWrapped()) {
                                    // the implementation will return exactly
                                    // the available width when text is wrapped.
                                    // Possibly with a small rounding
                                    // difference.
                                    REQUIRE_ALMOST_EQUAL(
                                        pThis->_pTextView->calcPreferredSize(
                                            Size(wrappedAtSecondPositionSize.width - 1, availableHeight)),
                                        Size(wrappedAtSecondPositionSize.width - 1, wrappedAtFirstPositionSize.height),
                                        Size(0.5, 0.5));
                                } else {
                                    REQUIRE_ALMOST_EQUAL(pThis->_pTextView->calcPreferredSize(Size(
                                                             wrappedAtSecondPositionSize.width - 1, availableHeight)),
                                                         wrappedAtFirstPositionSize, Size(1, 0));
                                }

                                REQUIRE_ALMOST_EQUAL(
                                    pThis->_pTextView
                                        ->calcPreferredSize(Size(wrappedAtFirstPositionSize.width, availableHeight))
                                        .width,
                                    wrappedAtFirstPositionSize.width, 1);
                            };
                        };
                    };
                }

                SECTION("availableWidth below single word width")
                {
                    _pTextView->setText("hello");

                    CONTINUE_SECTION_WHEN_IDLE(pThis, availableHeight)
                    {
                        Size unrestrictedSize =
                            pThis->_pTextView->calcPreferredSize(Size(Size::componentNone(), availableHeight));

                        // specifying an available width that is smaller than
                        // the word should not reduce the preferred size.
                        if (pThis->clipsPreferredWidthToAvailableWidth()) {
                            // this implementation will restrict the preferred
                            // width to the available width. This is not optimal
                            // behaviour, but with some implementations it
                            // cannot be avoided. so we accept it.
                            REQUIRE_ALMOST_EQUAL(
                                pThis->_pTextView->calcPreferredSize(Size(unrestrictedSize.width - 1, availableHeight)),
                                Size(unrestrictedSize.width - 1, unrestrictedSize.height),
                                Size(1, 1)); // the implementation might round
                                             // to the nearest real pixel (which
                                             // we assume is < 1 DIP)
                        } else if (pThis->wrapsAtCharacterBoundariesIfWordDoesNotFit()) {
                            // the implementation will wrap at character
                            // boundaries.
                            Size size =
                                pThis->_pTextView->calcPreferredSize(Size(unrestrictedSize.width - 1, availableHeight));

                            // width should be <= the specified width
                            REQUIRE(size.width <= unrestrictedSize.width - 1);

                            // should have gotten higher since wrapping occurred
                            REQUIRE(size.height > unrestrictedSize.height);
                        } else
                            REQUIRE(pThis->_pTextView->calcPreferredSize(
                                        Size(unrestrictedSize.width - 1, availableHeight)) == unrestrictedSize);
                    };
                }
            }

            void runPostInitTests() override
            {
                P<TestTextViewCore> pThis = this;

                TestViewCore::runPostInitTests();

                SECTION("text")
                {
                    SECTION("value")
                    {
                        _pTextView->setText("helloworld");

                        CONTINUE_SECTION_WHEN_IDLE(pThis) { pThis->verifyCoreText(); };
                    }
                }

                SECTION("preferredSize")
                {
                    SECTION("availableHeight=-1")
                    testPreferredSize(-1);
                    SECTION("availableHeight=0")
                    testPreferredSize(0);
                    SECTION("availableHeight=1")
                    testPreferredSize(1);
                    SECTION("availableHeight=1000")
                    testPreferredSize(1000);
                }
            }

            /** Verifies that the text view core's text has the expected value
                (the text set in the outer TextView object's TextView::text()
               property.*/
            virtual void verifyCoreText() = 0;

            P<TextView> _pTextView;
        };
    }
}

#endif