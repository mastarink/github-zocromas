# Copyright 1999-2013 Gentoo Foundation
# Distributed under the terms of the GNU General Public License v2
# $Header: $

EAPI=4

AUTOTOOLS_AUTORECONF=1

DESCRIPTION="mastar zoc transaction library"
HOMEPAGE="http://zocromas.mastarink.net/zocromas/${PN}.html"
SRC_URI="http://zocromas.mastarink.net/zocromas/${P}.tar.bz2"

LICENSE="GPL-2"
SLOT="0"
KEYWORDS="~amd64"
IUSE=""

DEPEND="dev-libs/glib:2
	mas-tar/zocromas_zoctypes
	mas-tar/zocromas_mas_wrap
	mas-tar/zocromas_mas_tools
	mas-tar/zocromas_mas_thtools
	mas-tar/zocromas_mas_thvariables
	mas-tar/zocromas_mas_varset
	mas-tar/zocromas_mas_log
	mas-tar/zocromas_mas_msg
	mas-tar/zocromas_mas_channel
	mas-tar/zocromas_mas_control
"

RDEPEND="${DEPEND}"

src_configure() {
    econf  --enable-silent-rules
}

src_install() {
    emake DESTDIR="${D}" install

#    dodoc FAQ NEWS README
#    dohtml EXTENDING.html ctags.html
}
