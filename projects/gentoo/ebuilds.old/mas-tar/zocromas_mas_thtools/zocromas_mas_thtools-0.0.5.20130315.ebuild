# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

AUTOTOOLS_AUTORECONF=1

DESCRIPTION="mastar zoc thread tools library"
HOMEPAGE="http://zocromas.mastarink.net/zocromas/thread_tools.html"
SRC_URI="http://zocromas.mastarink.net/zocromas/${P}.tar.bz2"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~amd64"
IUSE=""

DEPEND="dev-libs/glib:2 mas-tar/zocromas_zoctypes mas-tar/zocromas_mas_wrap"
RDEPEND="${DEPEND}"

src_configure() {
    econf  --enable-silent-rules
}

src_install() {
    emake DESTDIR="${D}" install

#    dodoc FAQ NEWS README
#    dohtml EXTENDING.html ctags.html
}